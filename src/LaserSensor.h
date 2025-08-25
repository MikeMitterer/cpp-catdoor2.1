#pragma once

//
// Created by Mike Mitterer on 2019-01-01.
//

#include "stdafx.h"
#include <VL53L0X.h>

// Laser-Sensor
const unsigned long REFRESH_LSENSOR_INTERVAL = 200; // ms
const unsigned long REFRESH_DISPLAY_INTERVAL = 800; // ms

// Weiter als 50cm macht keinen Sinn
const uint16_t MAX_DISTANCE = 500; // mm

// Sensor1 hat per default die Adresse 41 (0x29)
const uint8_t ADDRESS_SENSOR2 = 0x31;

extern ulong lastRefresh;
extern ulong lastDispRefresh;

extern uint8_t nextSensorToRefresh;

extern ulong distance1;
extern ulong distance2;

// Wenn diese Distanz unterschritten ist, wird die Türe bzw. die LED ausgelöst
extern uint16_t triggerDistance1;
extern uint16_t triggerDistance2;

void distSensorSetup(VL53L0X& distSensor1,VL53L0X& distSensor2);

