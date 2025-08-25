// AppAddress.h

#pragma once

// 1. Eigener Header ---------------------------------------------
#include "IPersistenceAddress.h"

// 5. System-/Standardbibliothek ---------------------------------
#include <optional>

class AppAddress final : public mm::IPersistenceAddress {
public:
    enum class Key : uint8_t {
        useSensor1 = 1,
        useSensor2 = 2,
        reactOnSensor = 3,
        lastSSID = 10 // nutzt 10..41
    };

    explicit AppAddress(const Key key) : _key(key) {}

    uint8_t asInt() const override {
        return static_cast<uint8_t>(_key);
    }

    const char* name() const override {
        return keyToName(_key);
    }

    static std::optional<Key> nameToKey(const std::string& name);
    static const char* keyToName(Key key);

private:
    Key _key;
};
