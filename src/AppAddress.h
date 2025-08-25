// AppPersistenceAddress.h
#pragma once
#include "IPersistenceAddress.h"

class AppAddress : public IPersistenceAddress {
public:
    enum class Key : uint8_t {
        useSensor1 = 1,
        useSensor2,
        reactOnSensor,
        lastSSID = 10
    };

    explicit AppAddress(Key key) : _key(key) {}

    uint8_t asInt() const override {
        return static_cast<uint8_t>(_key);
    }

    const char* name() const override {
        switch (_key) {
            case Key::useSensor1: return "useSensor1";
            case Key::useSensor2: return "useSensor2";
            case Key::reactOnSensor: return "reactOnSensor";
            case Key::lastSSID: return "lastSSID";
            default: return "unknown";
        }
    }

private:
    Key _key;
};
