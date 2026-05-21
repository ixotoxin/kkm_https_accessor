// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Unserialize/serialize WCS JSON object from/to std::wstring", "[json][wcs]") {
        using Catch::Matchers::WithinRel;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        std::wstring original { jsonObjStr };
        std::wstring created {};

        REQUIRE_NOTHROW(jsonDoc <<= original);
        REQUIRE_NOTHROW(jsonDoc >>= created);

        REQUIRE(removeWhiteSpace(std::move(original)) == removeWhiteSpace(std::move(created)));

        REQUIRE(jsonDoc.HasMember(nullKey));
        REQUIRE(jsonDoc[kv(nullKey)].IsNull());
        REQUIRE(jsonDoc.RemoveMember(nullKey));

        REQUIRE(jsonDoc.HasMember(emptyObjectKey));
        REQUIRE(jsonDoc[kv(emptyObjectKey)].IsObject());
        REQUIRE(jsonDoc[kv(emptyObjectKey)].ObjectEmpty());
        REQUIRE(jsonDoc.RemoveMember(emptyObjectKey));

        REQUIRE(jsonDoc.HasMember(emptyArrayKey));
        REQUIRE(jsonDoc[kv(emptyArrayKey)].IsArray());
        REQUIRE(jsonDoc[kv(emptyArrayKey)].Empty());
        REQUIRE(jsonDoc.RemoveMember(emptyArrayKey));

        REQUIRE(jsonDoc.HasMember(arrayKey));
        REQUIRE(jsonDoc[kv(arrayKey)].IsArray());
        REQUIRE(jsonDoc[kv(arrayKey)].Size() == 5);
        for (SizeType i {}; i < 5; ++i) {
            REQUIRE(jsonDoc[kv(arrayKey)][i].IsInt());
            REQUIRE(jsonDoc[kv(arrayKey)][i].GetInt() == static_cast<int>(i + 1));
        }
        REQUIRE(jsonDoc.RemoveMember(arrayKey));

        REQUIRE(jsonDoc.HasMember(trueKey));
        REQUIRE(jsonDoc[kv(trueKey)].IsBool());
        REQUIRE(jsonDoc[kv(trueKey)].GetBool() == trueVal);
        REQUIRE(jsonDoc.RemoveMember(trueKey));

        REQUIRE(jsonDoc.HasMember(trueStr1Key));
        REQUIRE(jsonDoc[kv(trueStr1Key)].IsString());
        REQUIRE(jsonDoc[kv(trueStr1Key)].GetString() == trueStr1Val);
        REQUIRE(jsonDoc.RemoveMember(trueStr1Key));

        REQUIRE(jsonDoc.HasMember(trueStr2Key));
        REQUIRE(jsonDoc[kv(trueStr2Key)].IsString());
        REQUIRE(jsonDoc[kv(trueStr2Key)].GetString() == trueStr2Val);
        REQUIRE(jsonDoc.RemoveMember(trueStr2Key));

        REQUIRE(jsonDoc.HasMember(trueStr3Key));
        REQUIRE(jsonDoc[kv(trueStr3Key)].IsString());
        REQUIRE(jsonDoc[kv(trueStr3Key)].GetString() == trueStr3Val);
        REQUIRE(jsonDoc.RemoveMember(trueStr3Key));

        REQUIRE(jsonDoc.HasMember(falseKey));
        REQUIRE(jsonDoc[kv(falseKey)].IsBool());
        REQUIRE(jsonDoc[kv(falseKey)].GetBool() == falseVal);
        REQUIRE(jsonDoc.RemoveMember(falseKey));

        REQUIRE(jsonDoc.HasMember(falseStr1Key));
        REQUIRE(jsonDoc[kv(falseStr1Key)].IsString());
        REQUIRE(jsonDoc[kv(falseStr1Key)].GetString() == falseStr1Val);
        REQUIRE(jsonDoc.RemoveMember(falseStr1Key));

        REQUIRE(jsonDoc.HasMember(falseStr2Key));
        REQUIRE(jsonDoc[kv(falseStr2Key)].IsString());
        REQUIRE(jsonDoc[kv(falseStr2Key)].GetString() == falseStr2Val);
        REQUIRE(jsonDoc.RemoveMember(falseStr2Key));

        REQUIRE(jsonDoc.HasMember(falseStr3Key));
        REQUIRE(jsonDoc[kv(falseStr3Key)].IsString());
        REQUIRE(jsonDoc[kv(falseStr3Key)].GetString() == falseStr3Val);
        REQUIRE(jsonDoc.RemoveMember(falseStr3Key));

        REQUIRE(jsonDoc.HasMember(zeroNumKey));
        REQUIRE(jsonDoc[kv(zeroNumKey)].IsInt());
        REQUIRE(jsonDoc[kv(zeroNumKey)].GetInt() == zeroNumVal);
        REQUIRE(jsonDoc.RemoveMember(zeroNumKey));

        REQUIRE(jsonDoc.HasMember(zeroStrKey));
        REQUIRE(jsonDoc[kv(zeroStrKey)].IsString());
        REQUIRE(jsonDoc[kv(zeroStrKey)].GetString() == zeroNumStrVal);
        REQUIRE(jsonDoc.RemoveMember(zeroStrKey));

        REQUIRE(jsonDoc.HasMember(fiveNumKey));
        REQUIRE(jsonDoc[kv(fiveNumKey)].IsInt());
        REQUIRE(jsonDoc[kv(fiveNumKey)].GetInt() == fiveNumVal);
        REQUIRE(jsonDoc.RemoveMember(fiveNumKey));

        REQUIRE(jsonDoc.HasMember(sevenStrKey));
        REQUIRE(jsonDoc[kv(sevenStrKey)].IsString());
        REQUIRE(jsonDoc[kv(sevenStrKey)].GetString() == sevenStrVal);
        REQUIRE(jsonDoc.RemoveMember(sevenStrKey));

        REQUIRE(jsonDoc.HasMember(pintNumKey));
        REQUIRE(jsonDoc[kv(pintNumKey)].IsInt());
        REQUIRE(jsonDoc[kv(pintNumKey)].GetInt() == pintNumVal);
        REQUIRE(jsonDoc.RemoveMember(pintNumKey));

        REQUIRE(jsonDoc.HasMember(pintStrKey));
        REQUIRE(jsonDoc[kv(pintStrKey)].IsString());
        REQUIRE(jsonDoc[kv(pintStrKey)].GetString() == pintStrVal);
        REQUIRE(jsonDoc.RemoveMember(pintStrKey));

        REQUIRE(jsonDoc.HasMember(nintNumKey));
        REQUIRE(jsonDoc[kv(nintNumKey)].IsInt());
        REQUIRE(jsonDoc[kv(nintNumKey)].GetInt() == nintNumVal);
        REQUIRE(jsonDoc.RemoveMember(nintNumKey));

        REQUIRE(jsonDoc.HasMember(nintStrKey));
        REQUIRE(jsonDoc[kv(nintStrKey)].IsString());
        REQUIRE(jsonDoc[kv(nintStrKey)].GetString() == nintStrVal);
        REQUIRE(jsonDoc.RemoveMember(nintStrKey));

        REQUIRE(jsonDoc.HasMember(pdblNumKey));
        REQUIRE(jsonDoc[kv(pdblNumKey)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(pdblNumKey)].GetDouble(), WithinRel(pdblNumVal, 0.000001));
        REQUIRE(jsonDoc.RemoveMember(pdblNumKey));

        REQUIRE(jsonDoc.HasMember(pdblStrKey));
        REQUIRE(jsonDoc[kv(pdblStrKey)].IsString());
        REQUIRE(jsonDoc[kv(pdblStrKey)].GetString() == pdblStrVal);
        REQUIRE(jsonDoc.RemoveMember(pdblStrKey));

        REQUIRE(jsonDoc.HasMember(ndblNumKey));
        REQUIRE(jsonDoc[kv(ndblNumKey)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(ndblNumKey)].GetDouble(), WithinRel(ndblNumVal, 0.000001));
        REQUIRE(jsonDoc.RemoveMember(ndblNumKey));

        REQUIRE(jsonDoc.HasMember(ndblStrKey));
        REQUIRE(jsonDoc[kv(ndblStrKey)].IsString());
        REQUIRE(jsonDoc[kv(ndblStrKey)].GetString() == ndblStrVal);
        REQUIRE(jsonDoc.RemoveMember(ndblStrKey));

        REQUIRE(jsonDoc.HasMember(strKey));
        REQUIRE(jsonDoc[kv(strKey)].IsString());
        REQUIRE(jsonDoc[kv(strKey)].GetString() == strVal);
        REQUIRE(jsonDoc.RemoveMember(strKey));

        REQUIRE(jsonDoc.HasMember(subKey));
        REQUIRE(jsonDoc[kv(subKey)].IsObject());

        REQUIRE(jsonDoc[kv(subKey)].HasMember(nullKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(nullKey)].IsNull());
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(nullKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(emptyObjectKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(emptyObjectKey)].IsObject());
        REQUIRE(jsonDoc[kv(subKey)][kv(emptyObjectKey)].ObjectEmpty());
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(emptyObjectKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(emptyArrayKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(emptyArrayKey)].IsArray());
        REQUIRE(jsonDoc[kv(subKey)][kv(emptyArrayKey)].Empty());
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(emptyArrayKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(arrayKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(arrayKey)].IsArray());
        REQUIRE(jsonDoc[kv(subKey)][kv(arrayKey)].Size() == 5);
        for (SizeType i {}; i < 5; ++i) {
            REQUIRE(jsonDoc[kv(subKey)][kv(arrayKey)][i].IsInt());
            REQUIRE(jsonDoc[kv(subKey)][kv(arrayKey)][i].GetInt() == static_cast<int>(i + 1));
        }
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(arrayKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(trueKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(trueKey)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv(trueKey)].GetBool() == trueVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(trueKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(trueStr1Key));
        REQUIRE(jsonDoc[kv(subKey)][kv(trueStr1Key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(trueStr1Key)].GetString() == trueStr1Val);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(trueStr1Key));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(trueStr2Key));
        REQUIRE(jsonDoc[kv(subKey)][kv(trueStr2Key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(trueStr2Key)].GetString() == trueStr2Val);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(trueStr2Key));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(trueStr3Key));
        REQUIRE(jsonDoc[kv(subKey)][kv(trueStr3Key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(trueStr3Key)].GetString() == trueStr3Val);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(trueStr3Key));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(falseKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(falseKey)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv(falseKey)].GetBool() == falseVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(falseKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(falseStr1Key));
        REQUIRE(jsonDoc[kv(subKey)][kv(falseStr1Key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(falseStr1Key)].GetString() == falseStr1Val);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(falseStr1Key));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(falseStr2Key));
        REQUIRE(jsonDoc[kv(subKey)][kv(falseStr2Key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(falseStr2Key)].GetString() == falseStr2Val);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(falseStr2Key));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(falseStr3Key));
        REQUIRE(jsonDoc[kv(subKey)][kv(falseStr3Key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(falseStr3Key)].GetString() == falseStr3Val);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(falseStr3Key));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(zeroNumKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(zeroNumKey)].IsInt());
        REQUIRE(jsonDoc[kv(subKey)][kv(zeroNumKey)].GetInt() == zeroNumVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(zeroNumKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(zeroStrKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(zeroStrKey)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(zeroStrKey)].GetString() == zeroNumStrVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(zeroStrKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(fiveNumKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(fiveNumKey)].IsInt());
        REQUIRE(jsonDoc[kv(subKey)][kv(fiveNumKey)].GetInt() == fiveNumVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(fiveNumKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(sevenStrKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(sevenStrKey)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(sevenStrKey)].GetString() == sevenStrVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(sevenStrKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(pintNumKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(pintNumKey)].IsInt());
        REQUIRE(jsonDoc[kv(subKey)][kv(pintNumKey)].GetInt() == pintNumVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(pintNumKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(pintStrKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(pintStrKey)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(pintStrKey)].GetString() == pintStrVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(pintStrKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(nintNumKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(nintNumKey)].IsInt());
        REQUIRE(jsonDoc[kv(subKey)][kv(nintNumKey)].GetInt() == nintNumVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(nintNumKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(nintStrKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(nintStrKey)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(nintStrKey)].GetString() == nintStrVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(nintStrKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(pdblNumKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(pdblNumKey)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(subKey)][kv(pdblNumKey)].GetDouble(), WithinRel(pdblNumVal, 0.000001));
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(pdblNumKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(pdblStrKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(pdblStrKey)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(pdblStrKey)].GetString() == pdblStrVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(pdblStrKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(ndblNumKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(ndblNumKey)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(subKey)][kv(ndblNumKey)].GetDouble(), WithinRel(ndblNumVal, 0.000001));
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(ndblNumKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(ndblStrKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(ndblStrKey)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(ndblStrKey)].GetString() == ndblStrVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(ndblStrKey));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(strKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(strKey)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(strKey)].GetString() == strVal);
        REQUIRE(jsonDoc[kv(subKey)].RemoveMember(strKey));

        REQUIRE(jsonDoc[kv(subKey)].ObjectEmpty());
        REQUIRE(jsonDoc.RemoveMember(subKey));
        REQUIRE(jsonDoc.ObjectEmpty());
    }
}
