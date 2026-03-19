// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_variables.h"
#include <lib/numeric.h>
#include <lib/text.h>
#include <lib/path.h>
#include <string>

namespace Server {
    void setVars(const Nln::Json & json) {
        Json::handleKey(
            json, "server",
            [] (const Nln::Json & json2, const std::wstring & path2) -> bool {
                Json::handleKey(json2, "ipv4Only", s_ipv4Only, path2);
                Json::handleKey(json2, "port", s_port, Numeric::between(c_minPort, c_maxPort), path2);
                Json::handleKey(
                    json2, "requestTimeout", s_requestTimeout,
                    Numeric::between(c_minRequestTimeout, c_maxRequestTimeout), path2
                );
                Json::handleKey(
                    json2, "concurrencyLimit", s_concurrencyLimit,
                    Numeric::between(c_minConcurrencyLimit, c_maxConcurrencyLimit), path2
                );
                Json::handleKey(json2, "enableLegacyTls", s_enableLegacyTls, path2);
                Json::handleKey(json2, "securityLevel", s_securityLevel, Numeric::between(0, 5), path2);
                Json::handleKey(
                    json2, "certificateChainFile", s_certificateChainFile,
                    Path::existsFile(Path::absolute(Path::noEmpty())), path2
                );
                Json::handleKey(
                    json2, "privateKeyFile", s_privateKeyFile,
                    Path::existsFile(Path::absolute(Path::noEmpty())), path2
                );
                Json::handleKey(json2, "privateKeyPassword", s_privateKeyPassword, path2);
                Json::handleKey(json2, "secret", s_secret, path2);
                Json::handleKey(json2, "loopbackWithoutSecret", s_loopbackWithoutSecret, path2);
                return true;
            }
        );
    }

    std::wostream & vars(std::wostream & stream) {
        std::wstring securityLevel;

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
            L"CFG: server.requestTimeout = " << s_requestTimeout << L"\n"
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
