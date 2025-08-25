//
// Created by Mike Mitterer on 2019-01-01.
//

#include <ArduinoLog.h>
#include "MyStepper.h"

#include "gpio.h"

namespace mm { namespace stepper {

//    void potiSetup();
    
//    void setup(AccelStepper& stepper, const Direction direction) {
//
////        pinMode(asInt(StepperPin::step), OUTPUT);
////        pinMode(asInt(StepperPin::direction), OUTPUT);
//
////        digitalWrite(asInt(StepperPin::step), LOW);
////
////        digitalWrite(asInt(StepperPin::direction),
////                     direction == Direction::Up
////                     ? static_cast<uint8_t>(HIGH)
////                     : static_cast<uint8_t>(LOW));
//
//        // Bevor der Speed abgefragt wird muss der Poti initialisiert sein
//        potiSetup();
//
//
//        // set the motorSpeed of the motor
//        setSpeed(stepper,getSpeed(), direction);
//
//        pinMode(asInt(StepperPin::sleepMode), OUTPUT);
//    }

//    void potiSetup() {
//        pinMode(asInt(StepperPin::speed), ANALOG);
//        pinMode(asInt(StepperPin::speed), INPUT);
//    }
//
//    long getSpeed() {
//        const long mappedSpeed = map(analogRead(asInt(StepperPin::speed)), 0, 4096, 0, MAX_SPEED);
//
//        // Log.notice(CR "Mapped-Speed: %l" CR, mappedSpeed);
//        return static_cast<int16_t>(mappedSpeed);
//    }
//
//    void setSpeed(AccelStepper& stepper, long speed, Direction direction) {
//        long speedToSet = speed;
//        if(direction == Direction::Down) {
//            speedToSet = speed * -1;
//        }
//
//        Log.notice(CR "Stepper-Speed set to: %d", speedToSet);
//        stepper.setSpeed(speedToSet);
//    }

    void on() {
        digitalWrite(asInt(StepperPin::sleepMode), LOW);
    }

    void off() {
        digitalWrite(asInt(StepperPin::sleepMode), HIGH);
    }
}}
