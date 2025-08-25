//
// Created by Mike Mitterer on 25.08.25.
//

#include "credential.h"

#include "AppAddress.h"

namespace mm::net {

    void CredentialManager::addCredential(const std::string& user, const std::string& pass) {
        credentials.emplace_back(user, pass);
    }

    void CredentialManager::loadState() {
        const auto tempUser = persistence.get(AppAddress(AppAddress::Key::lastSSID),"<undefined>").c_str();

        if (tempUser != "<undefined>") {
            lastSuccessfulUser = tempUser;
        }
    }

    void CredentialManager::markSuccess(const Credential& cred) {
        lastSuccessfulUser = cred.getSSID();
        persistence.set(AppAddress(AppAddress::Key::lastSSID), lastSuccessfulUser.c_str());
    }

    std::vector<Credential> CredentialManager::getOrderedCredentials() const {
        std::vector<Credential> result;

        // zuerst der zuletzt erfolgreiche (falls vorhanden)
        for (const auto& cred: credentials) {
            if (cred.ssid == lastSuccessfulUser) {
                result.push_back(cred);
                break;
            }
        }
        // dann alle anderen
        for (const auto& cred: credentials) {
            if (cred.ssid != lastSuccessfulUser) {
                result.push_back(cred);
            }
        }

        return result;
    }
}
