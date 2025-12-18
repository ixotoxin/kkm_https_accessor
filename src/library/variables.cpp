// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <lib/winapi.h>
#include <lib/datetime.h>
#include <log/variables.h>
#include <kkm/variables.h>
#include <config/defaults.h>
#include <config/variables.h>
#include <string>
#include <filesystem>

namespace Main {
    std::filesystem::path s_file {};
    std::filesystem::path s_directory {};
}

namespace Config {
    constexpr std::wstring_view c_directoryEnv { L"kkmha_conf_dir" };
    constexpr std::wstring_view c_directory { L"conf" };
    constexpr std::wstring_view c_file { L"kkmha.json" };
    std::filesystem::path s_directory {};
    std::filesystem::path s_file {};
}

namespace Log {
    constexpr std::wstring_view c_directoryEnv { L"kkmha_logs_dir" };
    constexpr std::wstring_view c_directory { L"logs" };
    constexpr ::DWORD c_eventId { 0 };
    constexpr ::WORD c_eventCategory { 0 };

    namespace Console {
        LevelUnderlying s_level { c_levelDebug };
        bool s_outputTimestamp { false };
        bool s_outputLevel { true };
    }

    namespace File {
        LevelUnderlying s_bgLevel { c_levelNone };
        LevelUnderlying s_fgLevel { c_levelInfo };
        std::filesystem::path s_directory { L"logs" };
    }

    namespace EventLog {
        LevelUnderlying s_bgLevel { c_levelNone };
        LevelUnderlying s_fgLevel { c_levelInfo };
    }

#ifdef DEBUG
    bool s_appendLocation { true };
#else
    bool s_appendLocation { false };
#endif
}

namespace Kkm {
    constexpr DateTime::SleepUnit c_sleepQuantum { DateTime::c_basicSleepQuantum }; // Миллисекунды
    constexpr std::wstring_view c_connParamsSeparator { L"," };
    const wchar_t c_separatorChar { L'-' };
    std::filesystem::path s_dbDirectory { L"kkm" };
    std::wstring s_defaultBaudRate { L"115200" };
    constexpr size_t c_minLineLength { 24 };
    constexpr size_t c_maxLineLength { 192 };
    constexpr size_t c_maxTextLength { 512 };
    size_t s_defaultLineLength { 42 };
    TimeZone s_timeZone { TimeZone::Device };
    bool s_timeZoneConfigured { false };
    constexpr FfdVersion c_defFallbackFfdVersion { FfdVersion::V_1_0_5 };
    FfdVersion s_fallbackFfdVersion { c_defFallbackFfdVersion };
    constexpr FfdVersionDetect c_ffdVersionDetect { FfdVersionDetect::Never };
    FfdVersionDetect s_ffdVersionDetect { c_ffdVersionDetect };
    constexpr DateTime::SleepUnit c_minDocumentClosingTimeout { DateTime::c_basicSleepQuantum }; // Миллисекунды
    constexpr DateTime::SleepUnit c_maxDocumentClosingTimeout { 10 * DateTime::c_basicSleep }; // Миллисекунды
    DateTime::SleepUnit s_documentClosingTimeout { DateTime::c_basicSleep }; // Миллисекунды
    std::wstring s_cliOperatorName { L"Оператор" };
    std::wstring s_cliOperatorInn {};
    std::wstring s_customerAccountField { L"Лицевой счёт (идентификатор для оплаты):" };
    constexpr double c_minCashInOut { 0.01 };
    constexpr double c_minMaxCashInOut { 1 };
    constexpr double c_maxMaxCashInOut { 1e9 };
    double s_maxCashInOut { 5e5 };
    constexpr double c_minPrice { 0.01 };
    constexpr double c_minMaxPrice { 1 };
    constexpr double c_maxMaxPrice { 1e9 };
    double s_maxPrice { 3e5 };
    constexpr double c_minQuantity { 0.001 };
    constexpr double c_minMaxQuantity { 1 };
    constexpr double c_maxMaxQuantity { 1e9 };
    double s_maxQuantity { 1e3 };
}
