// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "test_data.h"

namespace UnitTests::Wcs {
    // NOLINTBEGIN(modernize-use-constexpr)

    inline const std::wstring jsonPropSet {
        L"\"null\":null,"
        L"\"emptyObject\":{},"
        L"\"emptyArray\":[],"
        L"\"array\":[1,2,3,4,5],"
        L"\"true\":true,"
        L"\"trueStr1\":\"true\","
        L"\"trueStr2\":\"yes\","
        L"\"trueStr3\":\"enable\","
        L"\"false\":false,"
        L"\"falseStr1\":\"false\","
        L"\"falseStr2\":\"no\","
        L"\"falseStr3\":\"disable\","
        L"\"zeroNum\":0,"
        L"\"zeroStr\":\"0\","
        L"\"fiveNum\":5,"
        L"\"sevenStr\":\"seven\","
        L"\"pintNum\":12345,"
        L"\"pintStr\":\"12345\","
        L"\"nintNum\":-12345,"
        L"\"nintStr\":\"-12345\","
        L"\"pdblNum\":12345.67,"
        L"\"pdblStr\":\"12345.67\","
        L"\"ndblNum\":-12345.67,"
        L"\"ndblStr\":\"-12345.67\","
        L"\"str\":\"Lorem ipsum dolor sit amet...\""
    };

    inline const std::wstring jsonObjStr {
        L"{"s + jsonPropSet + L",\"sub\":{"s + jsonPropSet + L"}}"s
    };

    inline const auto nullKey = L"null"_key;
    inline const auto emptyObjectKey = L"emptyObject"_key;
    inline const auto emptyArrayKey = L"emptyArray"_key;
    inline const auto arrayKey = L"array"_key;
    inline const auto trueKey = L"true"_key;
    inline const auto trueStr1Key = L"trueStr1"_key;
    inline const auto trueStr2Key = L"trueStr2"_key;
    inline const auto trueStr3Key = L"trueStr3"_key;
    inline const auto falseKey = L"false"_key;
    inline const auto falseStr1Key = L"falseStr1"_key;
    inline const auto falseStr2Key = L"falseStr2"_key;
    inline const auto falseStr3Key = L"falseStr3"_key;
    inline const auto zeroNumKey = L"zeroNum"_key;
    inline const auto zeroStrKey = L"zeroStr"_key;
    inline const auto fiveNumKey = L"fiveNum"_key;
    inline const auto sevenStrKey = L"sevenStr"_key;
    inline const auto pintNumKey = L"pintNum"_key;
    inline const auto pintStrKey = L"pintStr"_key;
    inline const auto nintNumKey = L"nintNum"_key;
    inline const auto nintStrKey = L"nintStr"_key;
    inline const auto pdblNumKey = L"pdblNum"_key;
    inline const auto pdblStrKey = L"pdblStr"_key;
    inline const auto ndblNumKey = L"ndblNum"_key;
    inline const auto ndblStrKey = L"ndblStr"_key;
    inline const auto strKey = L"str"_key;
    inline const auto cookieKey = L"cookie"_key;
    inline const auto subKey = L"sub"_key;
    inline const auto enpKey = L"existingNumericProperty"_key;
    inline const auto etpKey = L"existingTextProperty"_key;
    inline const auto nepKey = L"nonExistingProperty"_key;
    inline const auto encodingTestKey = L"Тестовый ключ"_key;

    inline const std::wstring trueStr1Val { L"true" };
    inline const std::wstring trueStr2Val { L"yes" };
    inline const std::wstring trueStr3Val { L"enable" };
    inline const std::wstring falseStr1Val { L"false" };
    inline const std::wstring falseStr2Val { L"no" };
    inline const std::wstring falseStr3Val { L"disable" };
    inline const std::wstring zeroNumStrVal { L"0" };
    inline const std::wstring zeroStrVal { L"zero" };
    inline const std::wstring oneNumStrVal { L"1" };
    inline const std::wstring oneStrVal { L"one" };
    inline const std::wstring threeNumStrVal { L"3" };
    inline const std::wstring threeStrVal { L"three" };
    inline const std::wstring fiveNumStrVal { L"5" };
    inline const std::wstring fiveStrVal { L"five" };
    inline const std::wstring sevenNumStrVal { L"7" };
    inline const std::wstring sevenStrVal { L"seven" };
    inline const std::wstring nineNumStrVal { L"9" };
    inline const std::wstring nineStrVal { L"nine" };
    inline const std::wstring pintStrVal { L"12345" };
    inline const std::wstring nintStrVal { L"-12345" };
    inline const std::wstring pdblStrVal { L"12345.67" };
    inline const std::wstring ndblStrVal { L"-12345.67" };
    inline const std::wstring strVal { L"Lorem ipsum dolor sit amet..." };
    inline const std::wstring loweredStrVal { Text::lowered(strVal) };
    inline const std::wstring encodingTestVal { L"Тестовая строка" };
    inline const std::wstring_view cookieVal { L"Cookie" };
    inline const std::wstring loweredCookieVal { Text::lowered(cookieVal) };
    inline const std::wstring_view coalVal { L"Coal" };
    inline const std::wstring loweredCoalVal { Text::lowered(coalVal) };
    inline const std::wstring_view strCheckVal { L"Ut enim ad minim veniam..." };
    inline const std::wstring tmStrVal { DateTime::cast<std::wstring>(tmVal) };

    inline const std::unordered_set<std::wstring> strDomain {
        L"1", L"3", pintStrVal, L"5", trueStr2Val, L"7", sevenStrVal, L"9", nintStrVal, L"11", L"13", loweredStrVal
    };

    inline const std::unordered_set<std::wstring_view> svwDomain  {
        L"1", L"3", pintStrVal, L"5", falseStr2Val, L"7", sevenStrVal, L"9", nintStrVal, L"11", L"13", loweredStrVal
    };

    inline const std::unordered_map<int, std::wstring> i2sCastMap {
        { oneNumVal, oneStrVal },
        { threeNumVal, threeStrVal },
        { pintNumVal, std::wstring { cookieVal } },
        { fiveNumVal, fiveStrVal },
        { nintNumVal, std::wstring { coalVal } },
        { sevenNumVal, sevenStrVal },
        { nineNumVal, nineStrVal }
    };

    inline const std::unordered_map<std::wstring_view, int> v2iCastMap {
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

    inline const std::unordered_map<std::wstring, std::wstring_view> s2vCastMap {
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

    std::wstring strAddTilda(const std::wstring &);
    std::wstring strThrow(const std::wstring &);
    void prepareObject(Json::Wcs::Document &);
    std::wstring removeWhiteSpace(std::wstring);
    std::wstring removeTrailingZeros(std::wstring);
}
