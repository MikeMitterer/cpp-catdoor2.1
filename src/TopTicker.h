#pragma once

//
// Created by Mike Mitterer on 18.10.19.
//

// #ifndef CATDOOR2_TOPTICKER_H
// #define CATDOOR2_TOPTICKER_H

#include "CatDoor.h"

#include <cstdint>
#include <functional>

const auto MAX_UP_TIME_SECS = 8;

class TopTicker {
private:
    unsigned long oldMillis = 0;
    uint8_t maxUpTimeInSecs = 0;
    uint8_t secsInTopPosition  = 0;

public:
    TopTicker(uint8_t maxUpTime = MAX_UP_TIME_SECS);

    void tick(mm::Position currentPosition, std::function<void ()> triggerCallback);
    void reset();
};



// #endif //CATDOOR2_TOPTICKER_H
