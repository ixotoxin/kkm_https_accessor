// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "types.h"
#include <lib/datetime.h>
#include <string_view>

namespace Kkm {
    KKM_CONST(DateTime::SleepUnit, c_sleepQuantum, DateTime::c_basicSleepQuantum); // Миллисекунды
    KKM_CONST(std::wstring_view, c_serialNumberChars, L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.-_");
    KKM_CONST(std::wstring_view, c_connParamsSeparator, L",");
    KKM_CONST(wchar_t, c_separatorChar, L'-');
    KKM_CONST(std::wstring_view, c_dbDirectoryDef, L"kkm");
    KKM_CONST(std::wstring_view, c_defaultBaudRateDef, L"115200");
    KKM_CONST(size_t, c_defaultLineLengthDef, 42);
    KKM_CONST(size_t, c_defaultLineLengthMin, 24);
    KKM_CONST(size_t, c_defaultLineLengthMax, 192);
    KKM_CONST(size_t, c_maxTextLength, 512);
    KKM_CONST(FfdVersion, c_fallbackFfdVersionDef, FfdVersion::V_1_0_5);
    KKM_CONST(FfdVersionDetect, c_ffdVersionDetectDef, FfdVersionDetect::Once);
    KKM_CONST(DateTime::SleepUnit, c_documentClosingTimeoutDef, DateTime::c_basicSleep);
    KKM_CONST(DateTime::SleepUnit, c_documentClosingTimeoutMin, DateTime::c_basicSleepQuantum); // Миллисекунды
    KKM_CONST(DateTime::SleepUnit, c_documentClosingTimeoutMax, 10 * DateTime::c_basicSleep); // Миллисекунды
    KKM_CONST(std::wstring_view, c_cliOperatorNameDef, L"Оператор");
    KKM_CONST(std::wstring_view, c_cliOperatorInnDef, );
    KKM_CONST(std::wstring_view, c_customerAccountFieldDef, L"Лицевой счёт (идентификатор для оплаты):");
    KKM_CONST(double, c_minCashInOut, 0.01);
    KKM_CONST(double, c_maxCashInOutDef, 5e5);
    KKM_CONST(double, c_maxCashInOutMin, 1);
    KKM_CONST(double, c_maxCashInOutMax, 1e9);
    KKM_CONST(double, c_minPrice, 0.01);
    KKM_CONST(double, c_maxPriceDef, 3e5);
    KKM_CONST(double, c_maxPriceMin, 1);
    KKM_CONST(double, c_maxPriceMax, 1e9);
    KKM_CONST(double, c_minQuantity, 0.001);
    KKM_CONST(double, c_maxQuantityDef, 1e3);
    KKM_CONST(double, c_maxQuantityMin, 1);
    KKM_CONST(double, c_maxQuantityMax, 1e9);
}
