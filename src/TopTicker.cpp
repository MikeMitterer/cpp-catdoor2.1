//
// Created by Mike Mitterer on 18.10.19.
//

#include "TopTicker.h"

#include <Arduino.h>

TopTicker::TopTicker(uint8_t maxUpTime) :
    oldMillis{ millis() },
    maxUpTimeInSecs{ maxUpTime } {

}

/**
 * Zähler läuft solange die Tür in der Top-Position ist. Wenn die Maximalzeit erreicht ist
 * wird die Callback aufgerufen
 *
 * @param currentPosition
 * @param triggerCallback
 */
void TopTicker::tick(mm::Position currentPosition, std::function<void ()> triggerCallback) {
    auto currentMillis = millis();

    if(currentMillis - oldMillis > 1000) {
        oldMillis = currentMillis;
        if(currentPosition == mm::Position::Top) {
            secsInTopPosition++;
        }
    }
    
    if(secsInTopPosition > maxUpTimeInSecs) {
        secsInTopPosition = 0;
        triggerCallback();
    }
}

void TopTicker::reset() {
    secsInTopPosition = 0;
}
