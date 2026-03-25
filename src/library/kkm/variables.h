// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "defaults.h"
#include <filesystem>

namespace Kkm {
    KKM_MVARI(std::filesystem::path, s_dbDirectory, c_dbDirectoryDef);
    KKM_MVARI(std::wstring, s_defaultBaudRate, c_defaultBaudRateDef);
    KKM_MVARI(size_t, s_defaultLineLength, c_defaultLineLengthDef);
#if VERSION_LIMIT >= VERSION_10107
    KKM_MVARI(TimeZone, s_timeZone, TimeZone::Device);
    KKM_MVARI(bool, s_timeZoneConfigured, false);
#endif
    KKM_MVARI(FfdVersion, s_fallbackFfdVersion, c_fallbackFfdVersionDef);
    KKM_MVARI(FfdVersionDetect, s_ffdVersionDetect, c_ffdVersionDetectDef);
    KKM_MVARI(DateTime::SleepUnit, s_documentClosingTimeout, c_documentClosingTimeoutDef);
    KKM_MVARI(std::wstring, s_cliOperatorName, c_cliOperatorNameDef);
    KKM_MVARI(std::wstring, s_cliOperatorInn, c_cliOperatorInnDef);
    KKM_MVARI(std::wstring, s_customerAccountField, c_customerAccountFieldDef);
    KKM_MVARI(double, s_maxCashInOut, c_maxCashInOutDef);
    KKM_MVARI(double, s_maxPrice, c_maxPriceDef);
    KKM_MVARI(double, s_maxQuantity, c_maxQuantityDef);
}
