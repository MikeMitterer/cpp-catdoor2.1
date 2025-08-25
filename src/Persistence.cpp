//
// Created by Mike Mitterer on 14.10.19.
//

#include "Persistence.h"
#include "gpio.h"

#include <ArduinoLog.h>

void Persistence::init(uint8_t configVersion) {
    EEPROM.begin(EEPROM_SIZE);

    const auto version = EEPROM.read(asInt(Address::configVersion));
    if(version == 0) {
        writeDefaultValues(configVersion);
    } else if(version != configVersion) {
        upgrade(version, configVersion);
    }
    commit();
}

/**
 * Liest den Wert von der angegebenen Adresse.
 * Sollte der Wert im EEPROM nicht vorhanden sein wird der defaultValue zurückgegeben
 *
 * @param address
 * @param defaultValue
 *
 * @return Wert aus EEPROM oder defaultValue
 */
bool Persistence::get(Persistence::Address address) const {
    return read(address) != 0;
}

/**
 * Liest die Adresse aus dem EEPROM aus
 *
 * @param address
 * @return
 */
uint8_t Persistence::read(Persistence::Address address) const {
    return EEPROM.read(asInt(address));
}

/**
 * Wenn noch keine Daten im EEPROM stehen werden die Default-Daten gescrieben
 *
 * Die aktuelle Version wird ebenfalls gespeichert
 */
void Persistence::writeDefaultValues(uint8_t configVersion) const {
    EEPROM.write(asInt(Address::configVersion), configVersion);

    for (const auto& pair : defaultValues) {
        const auto setCallback = pair.second;
        setCallback(pair.first);
    }

}

/**
 * Wenn sich die Version ändert wird upgrade aufgerufen.
 *
 * Zur Zeit (Okt 2019) werden allerdings nur die default-Werte neu geschrieben
 */
void Persistence::upgrade(uint8_t fromVersion, uint8_t toVersion) const {
    writeDefaultValues(toVersion);
}

/**
 * Schreibt die Daten in das EEPROM
 */
void Persistence::set(Persistence::Address address, uint8_t value) {
    EEPROM.write(asInt(address), value);
}

/**
 * Die Adresse die als String übergeben wird, wird in eine richtige
 * "enum class Adresses" gemappet und dann der Wert geschrieben
 */
void Persistence::set(const std::string& address, uint8_t value) {
    const auto found = string2address.find(address);
    if (found != string2address.end()) {
        set(found->second, value);
        Log.notice( CR"%s:%d Saved!..." CR, address.c_str(), value);
    } else {
        Log.warning("Could not find '%s' Address-Mapping!", address.c_str());
    }
}

/**
 * Schreibt die Settings ins EEPROM
 * 
 * @return
 */
bool Persistence::commit() {
    return EEPROM.commit();
}
