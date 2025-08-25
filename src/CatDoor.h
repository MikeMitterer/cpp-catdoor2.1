#pragma once

//
// Created by Mike Mitterer on 2019-01-04.
//

//#ifndef CATDOOR2_CATDOOR_H
//#define CATDOOR2_CATDOOR_H


#include "application.h"
#include "gpio.h"
#include "MyStepper.h"
#include "SpeedPoti.h"

namespace mm {

    const auto MAX_OFF_THE_RAMP_MS = 300;

    enum class Position {
        Undefined,
        // Wenn die jeweiligen Endschalter erreicht sind
        Top, Bottom,
        // Wird in http.cpp bei onDoorUp und onDoorDown gesetzt
        OffTheRamp
    };

    class CatDoor {
    private:
        /**
         * Create an instance of the stepper class, specifying
         * the number of steps of the motor and the pins it's
         * attached to
         */
        AccelStepper stepper{ stepper::MotorType,
                         asInt(StepperPin::step),
                         asInt(StepperPin::direction) };

        SpeedPoti speedPoti;

        /** Motor speed */
        long speed{ -1 };

        unsigned long offTheRampMarker = 0;

    public:
        CatDoor();

        // Wenn sich einer der Endschalter löst wird die Position auch auf "Undefined" gesetzt
        Position position { Position::Undefined };

        stepper::Direction direction{ stepper::Direction::Up };

    public:
        void init();

        void setSpeed(long speed, boolean force = false);

        int16_t getSpeed();

        void up();

        void down();

        void move();

        void stop();
    };

}

// #endif //CATDOOR2_CATDOOR_H
