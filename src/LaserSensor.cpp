//
// Created by Mike Mitterer on 2019-01-02.
//

#include "LaserSensor.h"
#include "gpio.h"

uint8_t nextSensorToRefresh = 1;

ulong lastRefresh = 0;
ulong lastDispRefresh = 0;

ulong distance1 = 0;
ulong distance2 = 0;

// Distanz bei der Initialisierung - wird dann automatisch nachgerechnet
uint16_t triggerDistance1 = static_cast<uint16_t>(MAX_DISTANCE * 0.5);
uint16_t triggerDistance2 = static_cast<uint16_t>(MAX_DISTANCE * 0.5);

/**
 * Laser sensor pre-setup (Soll lt. GH vor Wire.begin kommen)
 *
 *      https://github.com/pololu/vl53l0x-arduino/issues/1
 */
void distSensorSetup(VL53L0X& distSensor1,VL53L0X& distSensor2) {
    pinMode(XSHUNT1_PIN, OUTPUT);
    pinMode(XSHUNT2_PIN, OUTPUT);

    delay(150);

//    // Address Sensor 1
//    pinMode(XSHUNT1_PIN, OUTPUT);
//
//    distSensor1.setAddress((uint8_t)0x29);
//    delay(150);
//
//    pinMode(XSHUNT1_PIN, INPUT);
//
//    delay(150);

    // Address Sensor 2

    pinMode(XSHUNT2_PIN, INPUT);
    delay(100);
    distSensor2.setAddress(ADDRESS_SENSOR2);
    delay(150);


    // Default Address
    pinMode(XSHUNT1_PIN, INPUT);
    // distSensor1.setAddress((uint8_t)0x29);
    // delay(150);

    distSensor1.init();
    distSensor2.init();


//    pinMode(XSHUNT2_PIN, OUTPUT);
//    digitalWrite(XSHUNT1_PIN, LOW);
//    digitalWrite(XSHUNT2_PIN, LOW);
//    delay(500);

    // Laser sensor 1
    //pinMode(XSHUNT1_PIN, INPUT);
//    delay(150);
//    delay(100);
    //distSensor1.setAddress((uint8_t)22); // 0x16
//    distSensor1.setTimeout(500);

    // Laser sensor 2
//    pinMode(XSHUNT2_PIN, INPUT);
    delay(150);
//    delay(100);
//    distSensor2.setAddress((uint8_t)23);
//    distSensor2.setTimeout(500);

    // Start continuous back-to-back mode (take readings as
    // fast as possible).  To use continuous timed mode
    // instead, provide a desired inter-measurement period in
    // ms (e.g. sensor.startContinuous(100)).
    distSensor1.startContinuous(50);
    distSensor2.startContinuous(50);
}
