// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <unordered_set>
#include <unordered_map>
#include <filesystem>
#include <lib/datetime.h>
#include <json/keys.h>

namespace UnitTests {
    using namespace std::string_literals;
    using namespace std::string_view_literals;
    using namespace std::chrono_literals;

    using namespace Json::Literals;
    using Json::SizeType;
    using Json::kv;

    // NOLINTBEGIN(modernize-use-constexpr)

    inline const bool trueVal { true };
    inline const bool falseVal { false };
    inline const int zeroNumVal { 0 };
    inline const int oneNumVal { 1 };
    inline const int threeNumVal { 3 };
    inline const int fiveNumVal { 5 };
    inline const int sevenNumVal { 7 };
    inline const int nineNumVal { 9 };
    inline const int pintNumVal { 12345 };
    inline const int nintNumVal { -12345 };
    inline const int intCheckVal { 53135 };
    inline const double pdblNumVal { 12345.67 };
    inline const double ndblNumVal { -12345.67 };
    inline const double dblCheckVal { 53135.79 };

    inline const std::tm tmVal {
        .tm_sec = 1, .tm_min = 2, .tm_hour = 3,
        .tm_mday = 4, .tm_mon = 4, .tm_year = 2026 - 1900,
        .tm_wday = 0, .tm_yday = 0, .tm_isdst = 0
    };

    inline const std::unordered_set<int> intDomain {
        -3, -1, 1, 3, 5, 7, 9, 11, 13, nintNumVal, pintNumVal
    };

    inline const std::unordered_map<int, double> i2dCastMap {
        { 1, 11.11 },
        { 3, 33.33 },
        { nintNumVal, ndblNumVal },
        { 5, 55.55 },
        { pintNumVal, pdblNumVal },
        { 7, 77.77 },
        { 9, 99.99 }
    };

    // NOLINTEND(modernize-use-constexpr)

    int intAdd10(int);
    int intThrow(int);
    std::filesystem::path tempFile();
}
