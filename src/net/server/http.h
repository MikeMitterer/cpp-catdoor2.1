#pragma once

//
// Created by Mike Mitterer on 2019-01-01.
//

//#ifndef CATDOOR2_HTTP_H
//#define CATDOOR2_HTTP_H

#include <ESPAsyncWebServer.h>
#include <Persistence.h>

namespace mm { namespace net {

    // Kommt aus http.cpp
    extern const uint8_t PORT;
    extern const uint8_t MAX_RETRIES;

    // Kommt aus http.cpp
    extern AsyncWebServer server;
    extern AsyncWebSocket ws;
    extern AsyncEventSource events;

    void onRequest(AsyncWebServerRequest* request);

    void onBody(AsyncWebServerRequest* request,
                uint8_t* data,
                size_t len,
                size_t index,
                size_t total);

    void onUpload(AsyncWebServerRequest* request,
                  String filename,
                  size_t index,
                  uint8_t* data,
                  size_t len,
                  bool final);

    void onGetHeap(AsyncWebServerRequest* request);
    void onGetConfig(AsyncWebServerRequest* request);

    void onSetConfig(AsyncWebServerRequest* request);

    void onDoorUp(AsyncWebServerRequest* request);
    void onDoorDown(AsyncWebServerRequest* request);
    void onDoorStop(AsyncWebServerRequest* request);
}}

//#endif //CATDOOR2_HTTP_H
