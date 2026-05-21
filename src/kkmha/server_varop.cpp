// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_json.h"
#include "server_variables.h"
#include <lib/numeric.h>
#include <lib/text.h>
#include <lib/path.h>
#include <string>

namespace Server {
    void setVars(const JsonVal & json) {
        Json::handleKey(
            json, L"server"_key,
            [] (const JsonVal & json2, const JsonPtr jptr2) -> bool {
                Json::handleKey(json2, L"ipv4Only"_key, s_ipv4Only, jptr2);
                Json::handleKey(json2, L"port"_key, s_port, Numeric::between(c_minPort, c_maxPort), jptr2);
                Json::handleKey(
                    json2, L"requestTimeout"_key, s_requestTimeout,
                    Numeric::between(c_minRequestTimeout, c_maxRequestTimeout), jptr2
                );
                Json::handleKey(
                    json2, L"poolSize"_key, s_poolSize,
                    Numeric::between(c_minPoolSize, c_maxPoolSize), jptr2
                );
                Json::handleKey(
                    json2, L"concurrencyLimit"_key, s_concurrencyLimit,
                    Numeric::between(c_minConcurrencyLimit, c_maxConcurrencyLimit), jptr2
                );
                Json::handleKey(json2, L"enableLegacyTls"_key, s_enableLegacyTls, jptr2);
                Json::handleKey(json2, L"securityLevel"_key, s_securityLevel, Numeric::between(0, 5), jptr2);
                Json::handleKey(
                    json2, L"certificateChainFile"_key, s_certificateChainFile,
                    Path::existsFile(Path::absolute(Path::noEmpty())), jptr2
                );
                Json::handleKey(
                    json2, L"privateKeyFile"_key, s_privateKeyFile,
                    Path::existsFile(Path::absolute(Path::noEmpty())), jptr2
                );
                Json::handleKey(json2, L"privateKeyPassword"_key, s_privateKeyPassword, jptr2);
                Json::handleKey(json2, L"secret"_key, s_secret, jptr2);
                Json::handleKey(json2, L"loopbackWithoutSecret"_key, s_loopbackWithoutSecret, jptr2);
                return true;
            }
        );
    }

    std::wostream & vars(std::wostream & stream) {
        std::wstring securityLevel;
        securityLevel.reserve(17);

        if (s_enableLegacyTls) {
            securityLevel.assign(L"0");
        } else if (s_securityLevel >= 0) {
            securityLevel.assign(std::to_wstring(s_securityLevel));
        } else {
            securityLevel.assign(L"<openssl-default>");
        }

        stream
            << L"CFG: server.ipv4Only = " << Text::Wcs::yesNo(s_ipv4Only) << L"\n"
            L"CFG: server.port = " << s_port << L"\n"
            L"CFG: server.requestTimeout = " << s_requestTimeout << L"s\n"
            L"CFG: server.poolSize = " << s_poolSize << L"\n"
            L"CFG: server.concurrencyLimit = " << s_concurrencyLimit << L"\n"
            L"CFG: server.enableLegacyTls = " << Text::Wcs::yesNo(s_enableLegacyTls) << L"\n"
            L"CFG: server.securityLevel = " << securityLevel << L"\n"
            L"CFG: server.certificateChainFile = \"" << s_certificateChainFile.wstring() << L"\"\n"
            L"CFG: server.privateKeyFile = \"" << s_privateKeyFile.wstring() << L"\"\n"
            L"CFG: server.privateKeyPassword = \"" << Text::convert(s_privateKeyPassword) << L"\"\n"
            L"CFG: server.secret = \"" << Text::convert(s_secret) << L"\"\n"
            L"CFG: server.loopbackWithoutSecret = " << Text::Wcs::yesNo(s_loopbackWithoutSecret) << L"\n";

        return stream;
    }
}
