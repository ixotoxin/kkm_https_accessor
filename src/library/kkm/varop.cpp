// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "varop.h"
#include "variables.h"
#include "strings.h"
#include "connparams.h"
#include "registry.h"
#include <lib/numeric.h>
#include <lib/text.h>
#include <lib/path.h>

namespace Kkm {
    void setVars(const JsonVal & json) {
        Json::handleKey(
            json, L"kkm"_key,
            [] (const JsonVal & json2, const JsonPtr jptr2) -> bool {
                Json::handleKey(
                    json2, L"dbDirectory"_key, s_dbDirectory,
                    Path::touchDir(Path::absolute(Path::noEmpty())), jptr2
                );
                Json::handleKey(json2, L"defaultBaudRate"_key, s_defaultBaudRate, Wcs::c_allowedBaudRate, jptr2);
                Json::handleKey(
                    json2, L"defaultLineLength"_key, s_defaultLineLength,
                    Numeric::between(c_defaultLineLengthMin, c_defaultLineLengthMax), jptr2
                );
#if VERSION_LIMIT >= VERSION_10107
                Json::handleKey(
                    json2, L"timeZone"_key, s_timeZone,
                    Wcs::c_timeZoneMap, [] (auto value) { s_timeZoneConfigured = true; return value; }, jptr2
                );
#endif
                Json::handleKey(json2, L"fallbackFfdVersion"_key, s_fallbackFfdVersion, Wcs::c_ffdVersionsMap, jptr2);
                Json::handleKey(json2, L"ffdVersionDetect"_key, s_ffdVersionDetect, Wcs::c_ffdVersionDetectMap, jptr2);
                Json::handleKey(
                    json2, L"documentClosingTimeout"_key, s_documentClosingTimeout,
                    DateTime::between(c_documentClosingTimeoutMin, c_documentClosingTimeoutMax), jptr2
                );
                Json::handleKey(
                    json2, L"cliOperator"_key,
                    [] (const JsonVal & json3, const JsonPtr jptr3) -> bool {
                        Json::handleKey(json3, L"name"_key, s_cliOperatorName, Text::Wcs::noEmpty(Text::Wcs::trim()), jptr3);
                        Json::handleKey(json3, L"inn"_key, s_cliOperatorInn, Text::Wcs::trim(), jptr3);
                        return true;
                    },
                    jptr2
                );
                Json::handleKey(json2, L"customerAccountField"_key, s_customerAccountField, jptr2);
                Json::handleKey(
                    json2, L"maxCashInOut"_key, s_maxCashInOut,
                    Numeric::between(c_maxCashInOutMin, c_maxCashInOutMax), jptr2
                );
                Json::handleKey(
                    json2, L"maxPrice"_key, s_maxPrice,
                    Numeric::between(c_maxPriceMin, c_maxPriceMax), jptr2
                );
                Json::handleKey(
                    json2, L"maxQuantity"_key, s_maxQuantity,
                    Numeric::between(c_maxQuantityMin, c_maxQuantityMax), jptr2
                );
                return true;
            }
        );
    }

    std::wostream & vars(std::wostream & stream) {
        stream
            << L"CFG: kkm.dbDirectory = \"" << s_dbDirectory.wstring() << L"\"\n"
            L"CFG: kkm.defaultBaudRate = " << s_defaultBaudRate << L"\n"
            L"CFG: kkm.defaultLineLength = " << s_defaultLineLength << L"\n"
#if VERSION_LIMIT >= VERSION_10107
            L"CFG: kkm.timeZone = tz" << Meta::toUnderlying(s_timeZone) << L"\n"
#endif
            L"CFG: kkm.fallbackFfdVersion = \"" << safeGet(Wcs::c_ffdVersions, s_fallbackFfdVersion) << L"\"\n"
            L"CFG: kkm.ffdVersionDetect = \"" << safeGet(Wcs::c_ffdVersionDetect, s_ffdVersionDetect) << L"\"\n"
            L"CFG: kkm.documentClosingTimeout = " << s_documentClosingTimeout.count() << L"ms\n"
            L"CFG: kkm.cliOperator.name = \"" << s_cliOperatorName << L"\"\n"
            L"CFG: kkm.cliOperator.inn = \"" << s_cliOperatorInn << L"\"\n"
            L"CFG: kkm.customerAccountField = \"" << s_customerAccountField << L"\"\n"
            L"CFG: kkm.maxCashInOut = " << s_maxCashInOut << L"\n"
            L"CFG: kkm.maxPrice = " << s_maxPrice << L"\n"
            L"CFG: kkm.maxQuantity = " << s_maxQuantity << L"\n"
            L"LRN: kkm.connParams = {\n";

        try {
            const std::filesystem::path directory { s_dbDirectory };
            bool nonFirst = false;
            for (auto const & entry: std::filesystem::directory_iterator { directory }) {
                if (entry.is_regular_file()) {
                    const std::filesystem::path & filePath { entry.path() };
                    auto fileExt = filePath.extension().wstring();
                    Text::lower(fileExt);
                    if (fileExt != L".json") {
                        continue;
                    }
                    auto serialNumber = Registry::serialNumber(filePath);
                    auto connParams = Registry::read(filePath);
                    if (nonFirst) {
                        stream << L",\n";
                    } else {
                        nonFirst = true;
                    }
                    stream
                        << L"LRN:     \"" << serialNumber
                        << L"\": \"" << static_cast<ConnParamString>(*connParams) << L'"';
                }
            }
        } catch (...) {}

        stream << L"\nLRN: }\n";

        return stream;
    }
}
