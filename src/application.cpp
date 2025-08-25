//
// Created by Mike Mitterer on 2019-01-02.
//

#include "application.h"
#include "gpio.h"
#include "LaserSensor.h"
#include "ServiceLocator.h"
#include "MyStepper.h"

#include <Arduino.h>
#include <Wire.h>
#include <ArduinoLog.h>


mm::Position prevPosition = mm::Position::Undefined;

void onPressTopBtn() {
    auto door = mm::ServiceLocator::getCatDoor();

    if(prevPosition != door->position) {
        Log.notice("onPressTopBtn, Pos: %d\n", door->position);
        prevPosition = door->position;
    }

    // Die Position kann nur auf "Undefined" sein wenn die
    // Tür vor diesem Event in Bewegung war
    if(door->position != mm::Position::OffTheRamp) {
        mm::ServiceLocator::getApplication()->state = State::MotorOff;

        door->position = mm::Position::Top;
    }
    door->direction = mm::stepper::Direction::Down;
}

void onReleaseTopBtn() {
    Log.notice("onReleaseTopBtn\n");

    // mm::ServiceLocator::getCatDoor()->position = mm::Position::Undefined;
}

void onPressBottomBtn() {
    auto door = mm::ServiceLocator::getCatDoor();

    if(prevPosition != door->position) {
        Log.notice("onPressBottomBtn, Pos: %d\n", door->position);
        prevPosition = door->position;
    }

    // Die Position kann nur auf "Undefined" sein wenn die
    // Tür vor diesem Event in Bewegung war
    if(door->position != mm::Position::OffTheRamp) {
        mm::ServiceLocator::getApplication()->state = State::MotorOff;

        door->position = mm::Position::Bottom;
    }
    door->direction = mm::stepper::Direction::Up;
}

void onReleaseBottomBtn() {
    Log.notice("onReleaseBottomBtn\n");

    // mm::ServiceLocator::getCatDoor()->position = mm::Position::Undefined;
}

void i2cWireSetup() {
    delay(150);
    
    Wire.begin(SDA_PIN,SCL_PIN);
    Wire.setClock(1000);

    delay(150);
}


void buttonSetup(OneButton& button,const uint8_t id, ButtonCallback onClick, ButtonCallback onRelease) {
    pinMode(id, INPUT);
    button.setPressMs(50);
    button.attachDuringLongPress(onClick);
    button.attachLongPressStop(onRelease);
}

void showAppInfo(Display& display, const std::string& projectName, const std::string& version, const bool show) {
    display.clear(Display::Page::one);

    delay(10);
    display.write(Display::Page::one, Display::Line::nr1,
                   "App: " +
                   projectName + " / " +
                   version);

    display.write(Display::Page::one, Display::Line::nr2,
                   "IP:  " + std::string(WiFi.localIP().toString().c_str()));

    // display.write(Display::Page::one, Display::Line::nr3,
    //                "MAC: " + std::string(WiFi.macAddress().c_str()));

    if(show) {
        display.show(Display::Page::one);
    }
}

