//
// Created by Mike Mitterer on 10.11.19.
//

#include "SpeedPoti.h"
#include "stdafx.h"
#include "gpio.h"

SpeedPoti::SpeedPoti(AccelStepper& stepper, StepperPin speedPin )
        : speedPin{static_cast<uint8_t>(asInt(speedPin))}, stepper{stepper} {
}

void SpeedPoti::init() {
    pinMode(speedPin, ANALOG);
    pinMode(speedPin, INPUT);

    // Init der MAX-Speed
    stepper.setMaxSpeed(MAX_SPEED);
}

int16_t SpeedPoti::getSpeed() {
    const long mappedSpeed = map(analogRead(speedPin), 0, 4096, 0, MAX_SPEED);
    const float EMA_a = 0.6;      //initialization of EMA alpha

    EMA_S = (EMA_a * mappedSpeed) + ((1 - EMA_a) * EMA_S);

    // Log.notice(CR "Mapped-Speed: %l" CR, mappedSpeed);
    return static_cast<int16_t>(EMA_S);
}

