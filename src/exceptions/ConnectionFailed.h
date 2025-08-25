#pragma once

#include <utility>

//
// Created by Mike Mitterer on 2019-01-03.
//

//#ifndef CATDOOR2_CONNECTIONFAILED_H
//#define CATDOOR2_CONNECTIONFAILED_H


#include <exception>
#include <string>

class ConnectionFailed : std::exception {
private:
    const std::string reason;

public:
    explicit ConnectionFailed(std::string reason) :
            reason{std::move(reason)} {}

    const char* what() const noexcept override {
        return reason.c_str();
    }

};


//#endif //CATDOOR2_CONNECTIONFAILED_H
