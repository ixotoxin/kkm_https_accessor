// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "test_data.h"

namespace UnitTests::Mbs {
    // NOLINTBEGIN(modernize-use-constexpr)

    inline const std::string jsonPropSet {
        "\"null\":null,"
        "\"emptyObject\":{},"
        "\"emptyArray\":[],"
        "\"array\":[1,2,3,4,5],"
        "\"true\":true,"
        "\"trueStr1\":\"true\","
        "\"trueStr2\":\"yes\","
        "\"trueStr3\":\"enable\","
        "\"false\":false,"
        "\"falseStr1\":\"false\","
        "\"falseStr2\":\"no\","
        "\"falseStr3\":\"disable\","
        "\"zeroNum\":0,"
        "\"zeroStr\":\"0\","
        "\"fiveNum\":5,"
        "\"sevenStr\":\"seven\","
        "\"pintNum\":12345,"
        "\"pintStr\":\"12345\","
        "\"nintNum\":-12345,"
        "\"nintStr\":\"-12345\","
        "\"pdblNum\":12345.67,"
        "\"pdblStr\":\"12345.67\","
        "\"ndblNum\":-12345.67,"
        "\"ndblStr\":\"-12345.67\","
        "\"str\":\"Lorem ipsum dolor sit amet...\""
    };

    inline const std::string jsonObjStr {
        "{"s + jsonPropSet + ",\"sub\":{"s + jsonPropSet + "}}"s
    };

    inline const auto nullKey = "null"_key;
    inline const auto emptyObjectKey = "emptyObject"_key;
    inline const auto emptyArrayKey = "emptyArray"_key;
    inline const auto arrayKey = "array"_key;
    inline const auto trueKey = "true"_key;
    inline const auto trueStr1Key = "trueStr1"_key;
    inline const auto trueStr2Key = "trueStr2"_key;
    inline const auto trueStr3Key = "trueStr3"_key;
    inline const auto falseKey = "false"_key;
    inline const auto falseStr1Key = "falseStr1"_key;
    inline const auto falseStr2Key = "falseStr2"_key;
    inline const auto falseStr3Key = "falseStr3"_key;
    inline const auto zeroNumKey = "zeroNum"_key;
    inline const auto zeroStrKey = "zeroStr"_key;
    inline const auto fiveNumKey = "fiveNum"_key;
    inline const auto sevenStrKey = "sevenStr"_key;
    inline const auto pintNumKey = "pintNum"_key;
    inline const auto pintStrKey = "pintStr"_key;
    inline const auto nintNumKey = "nintNum"_key;
    inline const auto nintStrKey = "nintStr"_key;
    inline const auto pdblNumKey = "pdblNum"_key;
    inline const auto pdblStrKey = "pdblStr"_key;
    inline const auto ndblNumKey = "ndblNum"_key;
    inline const auto ndblStrKey = "ndblStr"_key;
    inline const auto strKey = "str"_key;
    inline const auto cookieKey = "cookie"_key;
    inline const auto subKey = "sub"_key;
    inline const auto enpKey = "existingNumericProperty"_key;
    inline const auto etpKey = "existingTextProperty"_key;
    inline const auto nepKey = "nonExistingProperty"_key; // TODO: добавить тестов с несуществующим свойством
    inline const auto encodingTestKey = "Тестовый ключ"_key;

    inline const std::string trueStr1Val { "true" };
    inline const std::string trueStr2Val { "yes" };
    inline const std::string trueStr3Val { "enable" };
    inline const std::string falseStr1Val { "false" };
    inline const std::string falseStr2Val { "no" };
    inline const std::string falseStr3Val { "disable" };
    inline const std::string zeroNumStrVal { "0" };
    inline const std::string zeroStrVal { "zero" };
    inline const std::string oneNumStrVal { "1" };
    inline const std::string oneStrVal { "one" };
    inline const std::string threeNumStrVal { "3" };
    inline const std::string threeStrVal { "three" };
    inline const std::string fiveNumStrVal { "5" };
    inline const std::string fiveStrVal { "five" };
    inline const std::string sevenNumStrVal { "7" };
    inline const std::string sevenStrVal { "seven" };
    inline const std::string nineNumStrVal { "9" };
    inline const std::string nineStrVal { "nine" };
    inline const std::string pintStrVal { "12345" };
    inline const std::string nintStrVal { "-12345" };
    inline const std::string pdblStrVal { "12345.67" };
    inline const std::string ndblStrVal { "-12345.67" };
    inline const std::string strVal { "Lorem ipsum dolor sit amet..." };
    inline const std::string loweredStrVal { Text::lowered(strVal) };
    inline const std::string encodingTestVal { "Тестовая строка" };
    inline const std::string_view cookieVal { "Cookie" };
    inline const std::string loweredCookieVal { Text::lowered(cookieVal) };
    inline const std::string_view coalVal { "Coal" };
    inline const std::string loweredCoalVal { Text::lowered(coalVal) };
    inline const std::string_view strCheckVal { "Ut enim ad minim veniam..." };
    inline const std::string tmStrVal { DateTime::cast<std::string>(tmVal) };

    inline const std::unordered_set<std::string> strDomain {
        "1", "3", pintStrVal, "5", trueStr2Val, "7", sevenStrVal, "9", nintStrVal, "11", "13", loweredStrVal
    };

    inline const std::unordered_set<std::string_view> svwDomain  {
        "1", "3", pintStrVal, "5", falseStr2Val, "7", sevenStrVal, "9", nintStrVal, "11", "13", loweredStrVal
    };

    inline const std::unordered_map<int, std::string> i2sCastMap {
        { oneNumVal, oneStrVal },
        { threeNumVal, threeStrVal },
        { pintNumVal, std::string { cookieVal } },
        { fiveNumVal, fiveStrVal },
        { nintNumVal, std::string { coalVal } },
        { sevenNumVal, sevenStrVal },
        { nineNumVal, nineStrVal }
    };

    inline const std::unordered_map<std::string_view, int> v2iCastMap {
        { oneStrVal, oneNumVal },
        { pintStrVal, nintNumVal },
        { threeStrVal, threeNumVal },
        { trueStr2Val, oneNumVal },
        { fiveStrVal, fiveNumVal },
        { falseStr2Val, zeroNumVal },
        { sevenStrVal, sevenNumVal },
        { nintStrVal, pintNumVal },
        { nineStrVal, nineNumVal }
    };

    inline const std::unordered_map<std::string, std::string_view> s2vCastMap {
        { oneStrVal, oneNumStrVal },
        { pintStrVal, cookieVal },
        { threeStrVal, threeNumStrVal },
        { trueStr2Val, trueStr1Val },
        { fiveStrVal, fiveNumStrVal },
        { falseStr2Val, falseStr1Val },
        { sevenStrVal, sevenNumStrVal },
        { nintStrVal, coalVal },
        { nineStrVal, nineNumStrVal }
    };

    // NOLINTEND(modernize-use-constexpr)

    std::string strAddTilda(const std::string &);
    std::string strThrow(const std::string &);
    void prepareObject(Json::Mbs::Document &);
    std::string removeWhiteSpace(std::string);
    std::string removeTrailingZeros(std::string);
}
