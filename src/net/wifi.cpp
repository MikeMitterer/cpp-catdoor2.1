//
// Created by Mike Mitterer on 2019-01-03.
//

#include "wifi.h"
#include "exceptions/ConnectionFailed.h"

#include <WiFi.h>

#include <net/server/http.h>
#include <gpio.h>
#include <sstream>


namespace mm { namespace net {

    void initWifi(const std::string& ssid, const std::string& password, OneLED& indicator) {
        WiFi.begin(ssid.c_str(), password.c_str());

        int retry = 0;

        WiFiClass::mode(WIFI_STA);
        while (WiFiClass::status() != WL_CONNECTED && retry < net::MAX_RETRIES) {
            indicator.on();
            delay(100);
            indicator.off();
            Serial.print(".");
            retry++;
        }

        if (retry >= net::MAX_RETRIES) {
            std::stringstream message;

            message << "Could not connect to " << ssid;
            throw ConnectionFailed(message.str());
        }
    indicator.on();
    }

}}
