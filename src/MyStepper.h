#pragma once

//
// Created by Mike Mitterer on 2019-01-01.
//

//#ifndef CATDOOR2_MYSTEPPER_H
//#define CATDOOR2_MYSTEPPER_H

#include <cstdint>

#include "AccelStepper.h"

namespace mm { namespace stepper {

    // In der Regel bewegen sich die Motoren 1.8Grad pro move
    // 360 / 1.8 = 200
    const uint8_t STEPS_PER_REVOLUTION = 200;

    const uint8_t MotorType = 1;

    enum class Direction {
        Up, Down
    };

//    void setup(AccelStepper& stepper, Direction direction);
//    void setSpeed(AccelStepper& stepper, long speed, Direction direction);
//    long getSpeed();

    void on();
    void off();
}}

//#endif //CATDOOR2_MYSTEPPER_H
