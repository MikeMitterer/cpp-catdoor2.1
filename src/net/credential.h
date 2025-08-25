#pragma once

// 3. Eigene dependencies ----------------------------------------
#include <Persistence.h>

// 4. Fremdbibliothek --------------------------------------------

// 5. System-/Standardbibliothek ---------------------------------
#include <string>
#include <vector>
#include <optional>
#include <fstream>
#include <iostream>

namespace mm::net {

    class Credential {
    private:
        std::string ssid;
        std::string password;
        friend class CredentialManager;

    public:
        Credential(const std::string& ssid, const std::string& pass)
            : ssid(ssid), password(pass) {}

        const std::string& getSSID() const { return ssid; }
        const std::string& getPassword() const { return password; }

    };

    class CredentialManager {
    public:
        explicit CredentialManager(Persistence& persistenceRef)
            : persistence(persistenceRef) {
        }

        void addCredential(const std::string& user, const std::string& pass);

        void loadState();

        void markSuccess(const Credential& cred);

        std::vector<Credential> getOrderedCredentials() const;

        // Getter für die zuletzt erfolgreiche SSID
        const std::string& getLastSuccessfulSSID() const { return lastSuccessfulSSID; }

    private:
        std::vector<Credential> credentials;
        Persistence& persistence;
        std::string lastSuccessfulSSID;
    };

}
