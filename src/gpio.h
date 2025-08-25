#pragma once

//
// Created by Mike Mitterer on 2018-12-31.
//

//#ifndef CATDOOR2_GPIO_H
//#define CATDOOR2_GPIO_H

#include <OneButton.h>
#include <OneLED.h>

#ifdef BOARD_LOLIN32PRO
    const gpio_num_t pinInternalLED = GPIO_NUM_5;
#else
    const gpio_num_t internalLED = GPIO_NUM_2;
#endif

/*
                    LoLin32 PRO
                ┌────────────────┐
                │3V           GND│
                │EN            23│ Step (Speed)
                │36 (VP) (SCL) 22│ SCL (blau)
                │39 (VN)       TX│
                │34            RX│
   Speed (POTI) │35      (SDA) 21│ SDA (lila | grün)
      Led ERROR │32            19│
       Led OPEN │33            18│ XSHUNT2
    Led TRIGGER │25             5│
   Endswitch UP │26             4│ XSHUNT1
                │27             0│
 Endswitch DOWN │14             2│
                │12            15│ Direction
                │13           GND│ GND Stepper
                │GND           5V│
                │BAT          VIN│
                └────────────────┘
 */

// Stepper
//const uint8_t STEP_PIN = 2;
//const uint8_t DIRECTION_PIN = 15;
//const uint8_t SPEED_PIN = 34; // Poti

enum class StepperPin {
    step = 23,
    direction = 15,
    speed = 35,         // Poti
    sleepMode = 19      // LOW puts the driver in sleep mode
};

// Buttons
//const uint8_t BTN_UP_PIN = 26;
//const uint8_t BTN_DOWN_PIN = 14;

enum class ButtonPin {
    Up = 26,
    Down = 14
};

// Buttons müssen unbedingt als INPUT definiert/spezifiziert werden
//extern OneButton switchUp;
//extern OneButton buttonDown;

// LEDs
const uint8_t LED_ERROR = 32;

// - Türe offen
const uint8_t LED_OPEN = 33;

// - Wenn einer der beiden Tür-Sensoren aktiv wird
const uint8_t LED_TRIGGER = 25;

const uint8_t LED_WIFI = 27;

//extern OneLED ledError;
//extern OneLED ledOpen;
//extern OneLED ledI2C;

// I2C
const uint8_t SDA_PIN = 21;
const uint8_t SCL_PIN = 22;

// Laser-Sensor
const uint8_t XSHUNT1_PIN = 4;
const uint8_t XSHUNT2_PIN = 18;

// Dist-Range
const uint8_t DIST_RANGE1_PIN = 34;
const uint8_t DIST_RANGE2_PIN = 39;

/**
 * Konvertiert eine enum class in den dazugehörigen Int-Value
 *
 * Mehr:
 *      https://stackoverflow.com/a/33083231
 *
 * @tparam E
 * @param e
 * @return
 */
template <typename E>
constexpr typename std::underlying_type<E>::type asInt(E e) noexcept {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

//#endif //CATDOOR2_GPIO_H
