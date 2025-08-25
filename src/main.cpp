/**
 * Hello-World ESP32.
 * Blinkt eine LED und ermöglicht ein OAT update
 *
 * Wenn das OAT funktioniere soll muss im platformio.ini der Upload-Port gesetzt sein!
 *
 * Mehr:
 *      https://diyprojects.io/arduinoota-esp32-wi-fi-ota-wireless-update-arduino-ide/
 *
 * OTA mit platformio:
 *      http://docs.platformio.org/en/latest/platforms/espressif8266.html#over-the-air-ota-update
 *
 * Debug:
 * > https://www.esp32.com/viewtopic.php?t=263
 * > http://esp-idf.readthedocs.io/en/latest/get-started/idf-monitor.html#launch-gdb-for-gdbstub
 *
 *      /Users/mikemitterer/.platformio/packages/toolchain-xtensa32/bin/xtensa-esp32-elf-gdb ./.pioenvs/nodemcu-32s/firmware.elf -b 115200 -ex 'target remote /dev/cu.SLAB_USBtoUART'
 *      /Users/mikemitterer/.platformio/packages/toolchain-xtensa32/bin/xtensa-esp32-elf-gdb ./.pioenvs/nodemcu-32s/firmware.elf
 *      /Users/mikemitterer/.platformio/packages/toolchain-xtensa32/bin/xtensa-esp32-elf-addr2line -pfiaC -e ./.pioenvs/nodemcu-32s/firmware.elf
 */

// 1. Eigener Header ---------------------------------------------
#include "stdafx.h"

#include "application.h"
#include "CatDoor.h"
#include "config.h"
#include "gpio.h"
#include "LaserSensor.h"
#include "Display.h"
#include "ota.h"
#include "ServiceLocator.h"
#include "TopTicker.h"
#include "AppAddress.h"
#include "AppAddressKeys.h"

// 2. Projektinterner Header -------------------------------------
#include "net/server/http.h"
#include "net/server/SocketHandler.h"
#include "net/wifi.h"
#include "net/credential.h"
#include "exceptions/ConnectionFailed.h"

// 3. Eigene dependencies ----------------------------------------
#include "OneLED.h"
#include <Persistence.h>

// 4. Fremdbibliothek --------------------------------------------
#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <VL53L0X.h>

// 5. System-/Standardbibliothek ---------------------------------
#include <functional>


using namespace std::placeholders;
using namespace mm;

Display display;
void refreshDisplay();

// Default Address 41
VL53L0X distSensor1; // 0x29 NOLINT(cert-err58-cpp)
VL53L0X distSensor2; // NOLINT(cert-err58-cpp)

net::SocketHandler sh;

// Buttons müssen unbedingt als INPUT definiert/spezifiziert werden
OneButton buttonUp(asInt(ButtonPin::Up), false);
OneButton buttonDown(asInt(ButtonPin::Down), false);

OneLED ledError(LED_ERROR);
OneLED ledOpen(LED_OPEN);
OneLED ledTrigger(LED_TRIGGER);
OneLED ledWIFI(LED_WIFI);
// OneLED ledI2C(LED_I2C);

Persistence persistence;
TopTicker topTicker;

// Triggert die Türe extern (über HTTP - zum testen)
bool extDoorTrigger = false;

State prevAppState = State::Idle;

net::CredentialManager credManager(persistence);

void setup() {
    Serial.begin(115200);
    Log.begin(LOG_LEVEL_NOTICE, &Serial);

    persistence.setDefault(AppAddressKey::useSensor1, true);
    persistence.setDefault(AppAddressKey::useSensor2, true);
    persistence.setDefault(AppAddressKey::reactOnSensor, true);

    persistence.setUpgradeCallback([](uint8_t from, uint8_t to) {
        // eigene Logik bei Versionswechsel
    });

    persistence.init(Project_Config_VERSION);

    credManager.addCredential(Project_SSID_1, Project_PASSWORD_1);
    credManager.addCredential(Project_SSID_2, Project_PASSWORD_2);
    credManager.addCredential(Project_SSID_3, Project_PASSWORD_3);

    // Laden der letzten erfolgreichen Credentials
    credManager.loadState();

    // App wird initialisiert
    ServiceLocator::provide(std::make_shared<Application>());

    // Die CatDoor wird erzeugt und dem ServiceLocator übergeben
    ServiceLocator::provide(std::make_shared<CatDoor>())->init();

    // I2C Wire setup
    i2cWireSetup();

    // OLED setup
    display.init();

    // Dist-Sensor
    distSensorSetup(distSensor1,distSensor2);

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(pinInternalLED, OUTPUT);

    // Switches
    buttonSetup(buttonUp, asInt(ButtonPin::Up), onPressTopBtn, onReleaseTopBtn);
    buttonSetup(buttonDown, asInt(ButtonPin::Down), onPressBottomBtn, onReleaseBottomBtn);

    // Dist-Range Potis
    pinMode(DIST_RANGE1_PIN, INPUT);
    pinMode(DIST_RANGE2_PIN, INPUT);

    display.clear(Display::Page::one);
    display.write(Display::Page::one, Display::Line::nr1, "Connecting to...");

    // Connect to WIFI
    auto orderedCredentials = credManager.getOrderedCredentials();
    bool success = false;
    for (const auto& credential : orderedCredentials) {
        Log.info("Trying login: %s" CR, credential.getSSID().c_str());

        try {
            display.write(Display::Page::one, Display::Line::nr2, credential.getSSID());
            display.show(Display::Page::one);

            net::initWifi(credential.getSSID(),credential.getPassword(), ledWIFI);
            credManager.markSuccess(credential);
            success = true;
            break;

        } catch(const ConnectionFailed& e) {

            Log.error("%s" CR, e.what());
            delay(2000);
        }

    }
    if(!success) {
        display.clear(Display::Page::one);
        display.write(Display::Page::one,Display::Line::nr1, "Connection Failed!");
        display.write(Display::Page::one,Display::Line::nr2, "Rebooting...");
        display.show(Display::Page::one);

        delay(5000);
        ESP.restart();
    }

    digitalWrite(pinInternalLED, LOW);
    Log.notice("Connected to WIFI, %s" CR, credManager.getLastSuccessfulSSID().c_str());
    Log.notice(CR "IP: %s" CR, WiFi.localIP().toString().c_str());

    showAppInfo(display,Project_NAME, Project_VERSION);

    // OTA
    initOTA();
    ArduinoOTA.begin();

    // Definiert die Callbacks für HTTP-Requests
    // net::serverSetup(net::server);
    
    // attach AsyncEventSource
    net::server.addHandler(&net::events);

    // Catch-All Handlers
    // Any request that cannot find a Handler that canHandle it
    // ends in the callbacks below.
    net::server.onNotFound(net::onRequest);
    net::server.onFileUpload(net::onUpload);
    net::server.onRequestBody(net::onBody);

    // respond to GET requests on URL /heap
    net::server.on("/heap", HTTP_GET, net::onGetHeap);

    net::server.on("/door/up", HTTP_GET, net::onDoorUp);
    net::server.on("/door/down", HTTP_GET, net::onDoorDown);
    net::server.on("/motor/stop", HTTP_GET, net::onDoorStop);
    net::server.on("/config", HTTP_GET, net::onGetConfig);
    net::server.on("/config", HTTP_PUT, net::onSetConfig);
    net::server.on("/trigger", HTTP_GET, [](AsyncWebServerRequest* request) {
        extDoorTrigger = true;
        request->send(200, "text/plain", "OK");
    });

    // WebSocket
    //
    // Verbindet die Funktion onEvent der SocketHandler-Klasse schlussendlich mit
    // dem WebSocket
    //
    // Alte Variante:
    //      attach AsyncWebSocket
    //      ws.onEvent(onEvent);
    //
    auto callback = std::bind(&net::SocketHandler::onEvent, sh, _1, _2, _3, _4, _5, _6);
    net::ws.onEvent(callback);
    net::server.addHandler(&net::ws);

    // Run Server
    net::server.begin();

    // Save aller Settings
    persistence.commit();

    // display.clear(Display::Page::one);
    // Serial.println("Ready!");
}

void loop() {
    ArduinoOTA.handle();

    auto app = ServiceLocator::getApplication();
    auto door = ServiceLocator::getCatDoor();

    //auto currentState = ;
    auto speed = door->getSpeed();

    triggerDistance1 = map(analogRead(DIST_RANGE1_PIN),0, 4096,1, MAX_DISTANCE);
    triggerDistance2 = map(analogRead(DIST_RANGE2_PIN),0, 4096,1, MAX_DISTANCE);

    buttonUp.tick();
    buttonDown.tick();

    topTicker.tick(door->position,[&]() {
        door->down();
    });


    if(millis() - lastRefresh >= REFRESH_LSENSOR_INTERVAL) {
        if(prevAppState != app->state) {
            Log.notice("Refresh! State: %d" CR, app->state);
            prevAppState = app->state;
        }

        lastRefresh = millis();

        const bool useSensor1 = persistence.get(AppAddressKey::useSensor1, true);
        const bool useSensor2 = persistence.get(AppAddressKey::useSensor2, true);
        const bool reactOnSensor = persistence.get(AppAddressKey::reactOnSensor, true);

        if(nextSensorToRefresh == 1) {
            nextSensorToRefresh = 2;

            if(useSensor1) {
                distance1 = std::min(distSensor1.readRangeSingleMillimeters(), MAX_DISTANCE);
                if (distSensor1.timeoutOccurred()) { Serial.println(" TIMEOUT- Dist1"); }
            }

        } else {
            nextSensorToRefresh = 1;

            if(useSensor2) {
                distance2 = std::min(distSensor2.readRangeSingleMillimeters(), MAX_DISTANCE);
                if (distSensor2.timeoutOccurred()) { Serial.println(" TIMEOUT Dist2"); }
            }
        }

        if((distance1 < triggerDistance1 && useSensor1)
        || (distance2 < triggerDistance2 && useSensor2) || extDoorTrigger) {

            ledTrigger.on();

            extDoorTrigger = false;
            if(door->position != mm::Position::Top) {
                if(reactOnSensor) {
                    const auto force = app->state == State::MotorOn;
                    door->setSpeed(speed, force);
                    door->up();

                    Log.notice("Move up! Current position: %d" CR, door->position);
                } else {
                    Log.notice("Dist-Sensor triggered but 'reactOnSensor' was set to 'false'" CR);
                }
            }
            topTicker.reset();

        } else {
            ledTrigger.off();
        }

    }

    if(millis() - lastDispRefresh >= REFRESH_DISPLAY_INTERVAL) {
        lastDispRefresh = millis();
        if(app->state == State::Idle || app->state == State::MotorOff) {
            app->state = State::RefreshDisplay;
        }
    }


    switch (app->state) {

        case State::MotorOn:
            Log.verbose("Motor on, current position: %d" CR, door->position);
            // digitalWrite(pinInternalLED, LOW);

            door->setSpeed(speed);
            door->move();

            ledOpen.on();

            break;

        case State::MotorOff:
            if(prevAppState != app->state) {
                Log.notice("Motor off" CR);
                prevAppState = app->state;
            }
            
            // digitalWrite(pinInternalLED, HIGH);

            // Reset von SPEED - Damit wird beim nächsten MotorOn auf jeden Fall
            // die Direction richtig gesetzt
            door->setSpeed(-1);
            if(door->position == mm::Position::Bottom) {
                door->stop();
            }

            app->state = State::Idle;
            break;

        case State::RefreshDisplay:
            refreshDisplay();

            app->state = State::Idle;
            break;

        // Wird in der Regel
        // bei onReleaseUpBtn und onReleaseBottomBtn ausgelöst
        case State::Idle:
            ledError.off();
            ledOpen.off();
            delay(1);
            break;
    }

    // Wird z.B. von digitalWrite benötigt
    // - sonst geht die LED nicht an???
    delay(1);
}

void refreshDisplay() {
    auto app = ServiceLocator::getApplication();
    auto door = ServiceLocator::getCatDoor();

    //auto currentState = ;
    auto speed = door->getSpeed();

    auto outLine3 = std::string("Speed: ") + std::to_string(speed);
    auto outLine4 =
            std::string("D1: ") + std::to_string(distance1) + "mm D2: " + std::to_string(distance2) + "mm";

    auto outLine5 =
            std::string("T1: ") + std::to_string(triggerDistance1) + "mm T2: " + std::to_string(triggerDistance2) + "mm";

    // Log.notice("%s" CR, outLine4.c_str());
    // showAppInfo(display,Project_NAME, Project_VERSION, false);

    display.write(Display::Page::one, Display::Line::nr4, outLine3);
    display.write(Display::Page::one, Display::Line::nr5, outLine4);
    display.write(Display::Page::one, Display::Line::nr6, outLine5);

    display.show(Display::Page::one);
}
