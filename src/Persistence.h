#pragma once

//
// Created by Mike Mitterer on 14.10.19.
//

#include <EEPROM.h>

#include <functional>
#include <map>

/**
 * Handelt die Lese- und Schreibvorgänge ins EEPROM
 *
 * Das finale Schreiben findet in der Funktion "commit" statt
 */
class Persistence {
public:

    enum class Address {
        configVersion,
        useSensor1, useSensor2,
        reactOnSensor
    };

private:
    static const uint8_t EEPROM_SIZE = 10;
    static const uint8_t DEFAULT_VALUE = 255;

    std::map<Address, std::function<void (Address address)>> defaultValues = {
            { Address::useSensor1, [this](Address address) { set(address, true); } },
            { Address::useSensor2, [this](Address address) { set(address, true); } },
            { Address::reactOnSensor, [this](Address address) { set(address, true); } },
            };

    std::map<std::string, Address> string2address = {
            { "useSensor1", Address::useSensor1 },
            { "useSensor2", Address::useSensor2 },
            { "reactOnSensor", Address::reactOnSensor },
    };

public:
    void init(uint8_t configVersion);

    bool get(Address address) const;

    void set(Address address, uint8_t value);
    void set(const std::string& address, uint8_t value);

    bool commit();
private:
    uint8_t read(Address address) const;

    void writeDefaultValues(uint8_t configVersion) const;
    void upgrade(uint8_t fromVersion, uint8_t toVersion) const;
};

