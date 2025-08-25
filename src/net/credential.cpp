//
// Created by Mike Mitterer on 25.08.25.
//

#include "credential.h"

namespace mm::net {

    void CredentialManager::addCredential(const std::string& user, const std::string& pass) {
        credentials.emplace_back(user, pass);
    }

    void CredentialManager::loadState() {
        prefs.begin("login-data", true);
        lastSuccessfulUser = prefs.getString("last_user", "").c_str();
        prefs.end();
    }

    void CredentialManager::markSuccess(const Credential& cred) {
        lastSuccessfulUser = cred.getSSID();
        prefs.begin("login-data", false);
        prefs.putString("last_user", lastSuccessfulUser.c_str());
        prefs.end();
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
