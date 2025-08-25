#include "AppAddress.h"

// 5. System-/Standardbibliothek ---------------------------------
#include <optional>

namespace {
    const std::pair<AppAddress::Key, const char*> keyNameMap[] = {
        { AppAddress::Key::useSensor1, "useSensor1" },
        { AppAddress::Key::useSensor2, "useSensor2" },
        { AppAddress::Key::reactOnSensor, "reactOnSensor" },
        { AppAddress::Key::lastSSID, "lastSSID" }
    };
}

std::optional<AppAddress::Key> AppAddress::nameToKey(const std::string& name) {
    for (const auto& [key, str]: keyNameMap) {
        if (name == str) {
            return key;
        }
    }
    return std::nullopt;
}

const char* AppAddress::keyToName(Key key) {
    for (const auto& [k, str]: keyNameMap) {
        if (k == key) {
            return str;
        }
    }
    return "unknown";
}
