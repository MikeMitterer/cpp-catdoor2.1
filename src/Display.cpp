//
// Created by Mike Mitterer on 2019-01-01.
//

#include "Display.h"

// #include <utility> // für std::move

const uint8_t OLED_ADDRESS = 0x3c;

Display::Display() :
    u8g2( U8G2_R0,  /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL,  /* data=*/ SDA),
    oled {
            { "Screen1 / Line1", "Screen1 / Line2", "Screen1 / Line3", "Screen1 / Line4", "Screen1 / Line5", "Screen1 / Line6" },
            { "Screen2 / Line1", "Screen2 / Line2", "Screen2 / Line3", "Screen2 / Line4", "Screen2 / Line5", "Screen2 / Line6" },
            { "Screen3 / Line1", "Screen3 / Line2", "Screen3 / Line3", "Screen3 / Line4", "Screen3 / Line5", "Screen3 / Line6" },
        } {
}

void Display::init() {
    u8g2.begin();

    // Fonts: https://github.com/olikraus/u8g2/wiki/fntlistall
    if (OLED_MAX_LINES == 4) {
        // 6 Pixel height
        u8g2.setFont(u8g2_font_7x13_t_symbols);
    } else {
        // https://github.com/olikraus/u8g2/wiki/fntlist12
        u8g2.setFont(u8g2_font_6x13_tf);
    }

    u8g2.setFlipMode(1);

    delay(100);
}

void Display::clear(Page page) {
    const uint8_t pagenr = static_cast<int>(page);

    for (int line = 0; line < oled[pagenr].size(); line++) {
        oled[pagenr][line] = "";
    }
}

void Display::show(Page page) {
    const uint8_t pagenr = static_cast<int>(page);

    // clear the internal memory
    u8g2.clearBuffer();

    for (int line = 0; line < oled[pagenr].size(); line++) {
        // write something to the internal memory
        u8g2.drawStr(0, (line + 1) * (OLED_MAX_LINES + 4), oled[pagenr][line].c_str());
    }

    // transfer internal memory to the display
    u8g2.sendBuffer();
}


void Display::write(Page page, const std::string& text) {
    const uint8_t pagenr = static_cast<int>(page);

    int index = 0;
    for (; index < oled[pagenr].size() - 1; index++) {
        oled[pagenr][index] = oled[pagenr][index + 1];
    }
    oled[pagenr][index] = text;
}

void Display::write(Page page, Line line, const std::string& text) {
    const uint8_t pagenr = static_cast<int>(page);
    const uint8_t linenr = static_cast<int>(line);

    oled[pagenr][linenr] = text;
}

// - private ---------------------------------------------------------------------------------------

// int16_t Display::getLinePos(Display::Line line) {
//     return static_cast<uint16_t>(asInt(line) * lineHeight);
// }
//
// void Display::clearLine(int16_t y) {
//     ssd1306.setColor(BLACK);
//     ssd1306.fillRect(0, y, ssd1306.width(), lineHeight);
//     ssd1306.setColor(WHITE);
// }
