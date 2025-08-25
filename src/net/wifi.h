#pragma once

//
// Created by Mike Mitterer on 2019-01-03.
//

//#ifndef CATDOOR2_WIFI_H
//#define CATDOOR2_WIFI_H

#include <string>
#include "OneLED.h"

namespace mm { namespace net {


    void initWifi(const std::string& ssid, const std::string& password, OneLED& indicator);
    
}}

//#endif //CATDOOR2_WIFI_H
