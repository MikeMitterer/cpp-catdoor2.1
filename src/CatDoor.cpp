//
// Created by Mike Mitterer on 2019-01-04.
//

#include "CatDoor.h"
#include "ServiceLocator.h"

#include <ArduinoLog.h>

namespace mm {

    CatDoor::CatDoor() : speedPoti{ stepper, StepperPin::speed} {

    }

    void CatDoor::init() {

        // Basic Stepper settings
        // stepper::setup(stepper, direction);
        speedPoti.init();

        setSpeed(getSpeed());

        pinMode(asInt(StepperPin::sleepMode), OUTPUT);
    }

    /**
     * Schaltet den Motor ein und bewegt die Türe.
     *
     * In welche Richtung die Türe bewegt wird,
     */
    void CatDoor::move() {
        // auto maxSpeed = getSpeed();
        stepper::on();

        if(millis() - offTheRampMarker > MAX_OFF_THE_RAMP_MS) {
            mm::ServiceLocator::getCatDoor()->position = mm::Position::Undefined;
        }
        // if(speed < maxSpeed && mm::ServiceLocator::getCatDoor()->position == mm::Position::OffTheRamp) {
        //     setSpeed(speed + 10);
        // }

        stepper.runSpeed();
    }

    /**
     * Speed wird beim Stepper nur dann gesetzt wenn sich entweder die Speed geändert hat,
     * force auf TRUE ist oder Speed auf -1 ist (Init-State)
     */
    void CatDoor::setSpeed(long speed, boolean force) {
        // Damit wird beim nächsten "wirklichen" Speed change die
        // Speed wirklich gesetzt
        if(speed == -1) {
            this->speed = -1;
            return;
        }

        if(speed < this->speed - 10 || speed > this->speed + 10 || force) {
            this->speed = speed;

            long speedToSet = speed * -1;

            if(direction == mm::stepper::Direction::Down) {
                speedToSet = speed;
            }

//            long speedToSet = speed;
//
//            if(direction == mm::stepper::Direction::Down) {
//                speedToSet = speed * -1;
//            }

            Log.notice("Speed changed to %d, Direction: %s" CR, speedToSet,
                    this->direction == mm::stepper::Direction::Up ? "UP" : "Down");

            stepper.setSpeed(speedToSet);
        }
    }

    /**
     * Stoppt den Motor und setzt die Speed auf -1 (RESET)
     */
    void CatDoor::stop() {
        stepper::off();
        speed = -1;
    }

    /**
     * Triggert ein Tür-Hoch
     */
    void CatDoor::up() {
        direction = stepper::Direction::Up;

        if(position == mm::Position::Bottom) {
            position = mm::Position::OffTheRamp;
        }

        offTheRampMarker = millis();
        // setSpeed(getSpeed() - 200, true);
        ServiceLocator::getApplication()->state = State::MotorOn;
    }

    /**
     * Triggert ein Tür-Runter
     */
    void CatDoor::down() {
        direction = stepper::Direction::Down;

        if(position == mm::Position::Top) {
            position = mm::Position::OffTheRamp;
        }

        offTheRampMarker = millis();
        // setSpeed(getSpeed() - 200, true);
        ServiceLocator::getApplication()->state = State::MotorOn;
    }

    int16_t CatDoor::getSpeed() {
        return speedPoti.getSpeed();
    }
}
