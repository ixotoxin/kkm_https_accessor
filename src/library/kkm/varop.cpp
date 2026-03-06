// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "varop.h"
#include "defaults.h"
#include "variables.h"
#include "strings.h"
#include "connparams.h"
#include <lib/numeric.h>
#include <lib/text.h>
#include <lib/path.h>

namespace Kkm {
    void setVars(const Nln::Json & json) {
        Json::handleKey(
            json, "kkm",
            [] (const Nln::Json & json2, const std::wstring & path2) -> bool {
                Json::handleKey(
                    json2, "dbDirectory", s_dbDirectory,
                    Path::touchDir(Path::absolute(Path::noEmpty())), path2
                );
                Json::handleKey(json2, "defaultBaudRate", s_defaultBaudRate, Wcs::c_allowedBaudRate, path2);
                Json::handleKey(
                    json2, "defaultLineLength", s_defaultLineLength,
                    Numeric::between(c_minLineLength, c_maxLineLength), path2
                );
                Json::handleKey(
                    json2, "timeZone", s_timeZone,
                    Mbs::c_timeZoneMap, [] (auto value) { s_timeZoneConfigured = true; return value; }, path2
                );
                Json::handleKey(json2, "fallbackFfdVersion", s_fallbackFfdVersion, Mbs::c_ffdVersionsMap, path2);
                Json::handleKey(json2, "ffdVersionDetect", s_ffdVersionDetect, Mbs::c_ffdVersionDetectMap, path2);
                if (s_ffdVersionDetect == FfdVersionDetect::Once || s_ffdVersionDetect == FfdVersionDetect::Sometimes) {
                    throw Failure( // NOLINT(*-exception-baseclass)
                        KKM_WFMT(
                            Wcs::c_unsupportedValue2,
                            wcsSafeGet(Mbs::c_ffdVersionDetect, s_ffdVersionDetect),
                            path2,
                            L"ffdVersionDetect"
                        )
                    );
                }
                Json::handleKey(
                    json2, "documentClosingTimeout", s_documentClosingTimeout,
                    DateTime::between(c_minDocumentClosingTimeout, c_maxDocumentClosingTimeout), path2
                );
                Json::handleKey(
                    json2, "cliOperator",
                    [] (const Nln::Json & json3, const std::wstring & path3) -> bool {
                        Json::handleKey(json3, "name", s_cliOperatorName, Text::Wcs::noEmpty(Text::Wcs::trim()), path3);
                        Json::handleKey(json3, "inn", s_cliOperatorInn, Text::Wcs::trim(), path3);
                        return true;
                    },
                    path2
                );
                Json::handleKey(json2, "customerAccountField", s_customerAccountField, path2);
                Json::handleKey(
                    json2, "maxCashInOut", s_maxCashInOut,
                    Numeric::between(c_minMaxCashInOut, c_maxMaxCashInOut), path2
                );
                Json::handleKey(
                    json2, "maxPrice", s_maxPrice,
                    Numeric::between(c_minMaxPrice, c_maxMaxPrice), path2
                );
                Json::handleKey(
                    json2, "maxQuantity", s_maxQuantity,
                    Numeric::between(c_minMaxQuantity, c_maxMaxQuantity), path2
                );
                return true;
            }
        );
    }

    std::wostream & vars(std::wostream & stream) {
        stream
            << L"CFG: kkm.dbDirectory = \"" << s_dbDirectory.native() << L"\"\n"
            L"CFG: kkm.defaultBaudRate = " << s_defaultBaudRate << L"\n"
            L"CFG: kkm.defaultLineLength = " << s_defaultLineLength << L"\n"
            L"CFG: kkm.timeZone = tz" << Meta::toUnderlying(s_timeZone) << L"\n"
            L"CFG: kkm.fallbackFfdVersion = \"" << wcsSafeGet(Mbs::c_ffdVersions, s_fallbackFfdVersion) << L"\"\n"
            L"CFG: kkm.ffdVersionDetect = \"" << wcsSafeGet(Mbs::c_ffdVersionDetect, s_ffdVersionDetect) << L"\"\n"
            L"CFG: kkm.documentClosingTimeout = " << s_documentClosingTimeout << L"\n"
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
                    std::wstring fileExt { filePath.extension().native() };
                    Text::lower(fileExt);
                    if (fileExt != L".json") {
                        continue;
                    }
                    KnownConnParams connParams { entry.path() };
                    if (nonFirst) {
                        stream << L",\n";
                    } else {
                        nonFirst = true;
                    }
                    stream
                        << L"LRN:     \"" << connParams.serialNumber()
                        << L"\": \"" << static_cast<std::wstring>(connParams) << L'"';
                }
            }
        } catch (...) {}

        stream << L"\nLRN: }\n";

        return stream;
    }
}
