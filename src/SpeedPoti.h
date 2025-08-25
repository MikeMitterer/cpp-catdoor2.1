#pragma once

//
// Created by Mike Mitterer on 10.11.19.
//

// #ifndef CATDOOR2_POTI_H
// #define CATDOOR2_POTI_H



#include <stdint.h>
#include <AccelStepper.h>
#include "MyStepper.h"
#include "gpio.h"

class SpeedPoti {
    // Maximale Drehzahl die der Motor erreichen kann - GESCHÄTZT
    const uint16_t MAX_SPEED = 900;

private:
    uint8_t speedPin;
    AccelStepper& stepper;

    // Poti + Filter (Exponential Moving Average algorithm)
    //      Mehr: https://www.norwegiancreations.com/2015/10/tutorial-potentiometers-with-arduino-and-filtering/
    //initialization of EMA S
    uint16_t EMA_S = 0;

public:
    SpeedPoti(AccelStepper& stepper, StepperPin speedPin );

    void init();
    int16_t getSpeed();
};



// #endif //CATDOOR2_POTI_H
