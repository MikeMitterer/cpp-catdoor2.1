//
// Created by Mike Mitterer on 2019-01-01.
//


#include "http.h"
#include "config.h"
#include "ServiceLocator.h"
#include "Persistence.h"

#include <ArduinoJson.h>
#include <ArduinoLog.h>

#include <sstream>

#include "AppAddress.h"
#include "AppAddressKeys.h"

// Kommt aus main.cpp
extern mm::Persistence persistence;

namespace mm {
    namespace net {

        const uint8_t PORT = 80;
        const uint8_t MAX_RETRIES = 80;

        AsyncWebServer server(PORT);                                                   // NOLINT(cert-err58-cpp)
        AsyncWebSocket ws("/ws"); // access at ws://[esp ip]/ws                    // NOLINT(cert-err58-cpp)
        AsyncEventSource events("/events"); // event source (Server-Sent events)   // NOLINT(cert-err58-cpp)

        void onRequest(AsyncWebServerRequest *request) {
            //Handle Unknown Request
            request->send(404);
        }

        void onBody(AsyncWebServerRequest *request,
                    uint8_t *data,
                    size_t len,
                    size_t index,
                    size_t total) {
            //Handle body
        }

        void onUpload(AsyncWebServerRequest *request,
                      String filename,
                      size_t index,
                      uint8_t *data,
                      size_t len,
                      bool final) {
            //Handle upload
        }

        /**
         * Gibt den aktuellen Heap zurück
         *
         * Reminder: (Lambda)
         *    net::server.on("/heap", HTTP_GET, [](AsyncWebServerRequest* request) {
         *      request->send(200, "text/plain", String(ESP.getFreeHeap()));
         *    });
         *
         * Url:
         *      http://192.168.0.225/heap
         *
         * @param request
         */
        void onGetHeap(AsyncWebServerRequest *const request) {
            auto heap = ESP.getFreeHeap();
            auto heapInKB = heap / 1024;

            StaticJsonDocument<200> root;
            String message;

            root["heap"]["bytes"] = String(heap);
            root["heap"]["kb"] = String(heapInKB);;

            serializeJson(root, message);

            // request->send(200, "text/plain", ss.str().c_str());
            request->send(200, "text/plain", message);
        }

        /**
         * Gibt die aktuelle Config als JSON-String zurück
         *
         * @param request
         */
        void onGetConfig(AsyncWebServerRequest *const request) {
            StaticJsonDocument<500> root;
            String message;

            root["project"]["name"] = String(Project_NAME.c_str());
            root["project"]["version"] = String(Project_VERSION.c_str());

            root["config"]["version"] = String(Project_Config_VERSION);
            root["config"]["useSensor1"] = persistence.get(AppAddressKey::useSensor1, true);
            root["config"]["useSensor2"] = persistence.get(AppAddressKey::useSensor2, true);
            root["config"]["reactOnSensor"] = persistence.get(AppAddressKey::reactOnSensor, true);

            serializeJson(root, message);

            // request->send(200, "text/plain", ss.str().c_str());
            request->send(200, "text/plain", message);

        }

        /**
         * Schreibt die Daten in die Config
         *
         * @param request
         */
        void onSetConfig(AsyncWebServerRequest *const request) {
            const auto nrOfParams = request->params();
            if(nrOfParams > 0) {
                Log.notice(CR "Received #Params: %d," CR, nrOfParams);
                for(int pIndex = 0 ; pIndex < nrOfParams; pIndex++){
                    const AsyncWebParameter* param = request->getParam(pIndex);
                    const auto& name = param->name();
                    const auto& valueAsString = param->value();

                    uint8_t value = valueAsString.toInt();
                    Log.notice(CR "  - %s: %s," CR, name.c_str(), valueAsString.c_str());

                    if(valueAsString.equalsIgnoreCase("true") || valueAsString.equalsIgnoreCase("yes")) {
                        value = 1;
                    } else if(valueAsString.equalsIgnoreCase("false") || valueAsString.equalsIgnoreCase("no")){
                        value = 0;
                    }
                    auto mayBeKey = AppAddress::nameToKey(std::string(name.c_str()));
                    if(mayBeKey.has_value()) {
                        const AppAddress address(mayBeKey.value());
                        persistence.set(address, value);
                    }
                }
            }

            if(persistence.commit()) {
                request->send(204, "text/plain");
            } else {
                request->send(500, "text/plain", "EEPROM commit failed!");
            }
        }

        /**
         * Fährt die Tür nach oben
         *
         * Url:
         *      http://192.168.0.225/door/up
         *
         * @param request
         */
        void onDoorUp(AsyncWebServerRequest *const request) {
            auto door = ServiceLocator::getCatDoor();

            door->up();
            
            Log.notice(CR "onDoorUp, Pos: %d, Direction: %d" CR, door->position, door->direction);

            request->send(200, "text/plain", "OK");
        }

        /**
         * Fährt die Tür nach unten
         *
         * Url:
         *      http://192.168.0.225/door/down
         *
         * @param request
         */
        void onDoorDown(AsyncWebServerRequest *const request) {
            auto door = ServiceLocator::getCatDoor();

            door->down();

            Log.notice(CR "onDoorDown, Pos: %d, Direction: %d", door->position, door->direction);

            request->send(200, "text/plain", "OK");
        }

        /**
         * Triggert einen Tür-Event (zum testen)
         *
         * Url:
         *      http://192.168.0.225/trigger
         *
         * @param request
         */
        void onTrigger(AsyncWebServerRequest *const request) {
            auto door = ServiceLocator::getCatDoor();

            door->down();

            Log.notice(CR "onDoorDown, Pos: %d, Direction: %d", door->position, door->direction);

            request->send(200, "text/plain", "OK");
        }

        /**
         * Fährt die Tür nach oben
         *
         * Url:
         *      http://192.168.0.225/door/stop
         *
         * @param request
         */
        void onDoorStop(AsyncWebServerRequest *request) {
            ServiceLocator::getApplication()->state = State::MotorOff;

            request->send(200, "text/plain", "OK");
        }

    }
}
