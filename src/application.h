#pragma once

//
// Created by Mike Mitterer on 2019-01-02.
//

//#ifndef CATDOOR2_APPLICATION_H
//#define CATDOOR2_APPLICATION_H

#include <string>

#include <VL53L0X.h>
#include <OneButton.h>
#include <ESPAsyncWebServer.h>

#include "Display.h"

extern "C" {
    typedef void (*ButtonCallback)();
}

enum class State {
    Error, Idle,
    MotorOn, MotorOff,
    RefreshDisplay,
    // Up, Down
};

void onPressTopBtn();
void onReleaseTopBtn();
void onPressBottomBtn();
void onReleaseBottomBtn();

void i2cWireSetup() ;
void buttonSetup(OneButton& button, uint8_t id, ButtonCallback onClick, ButtonCallback onRelease);

void showAppInfo(Display& display, const std::string& projectName, const std::string& version, const bool show = true);

class Application {
public:

    /** Per default fährt die Türe hoch und initialisiert sich. */
    State state { State::MotorOff };
};

//#endif //CATDOOR2_APPLICATION_H
