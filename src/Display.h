#pragma once

//
// Created by Mike Mitterer on 2019-01-01.
//

//#ifndef CATDOOR2_OLED_H
//#define CATDOOR2_OLED_H

#include "gpio.h"

#include <cstdint>
#include <U8g2lib.h>

// OLED
// extern const uint8_t OLED_ADDRESS;

const uint8_t OLED_MAX_LINES = 6;
typedef std::array<std::string, OLED_MAX_LINES> OLED_PAGE;

class Display {
private:
    const uint8_t fontHeight = 10;
    const uint8_t lineHeight = static_cast<uint8_t>(fontHeight + 1);

    const uint8_t nrOfDisplays = 3;
    uint8_t activeDisplay = 0;

    // Diese Variante funkt auch mit dem Kompass!!!!
    // ESP32 Thing, HW I2C with pin remapping
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

    std::vector<OLED_PAGE> oled;

public:
    enum class Line { nr1, nr2, nr3, nr4, nr5, nr6 };
    enum class Page { one, two, three };

public:
    Display();

    void init();

    void clear(Page page);

    void write(Page page, const std::string& text);
    void write(Page page, Line line, const std::string& text);

    void show(Page page);

private:
    // void clearLine(int16_t y);
    // int16_t getLinePos(Line line);
};



//#endif //CATDOOR2_OLED_H
