// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Unserialize/serialize MBS JSON object from/to file", "[json][mbs]") {
        using Catch::Matchers::WithinRel;

        Json::Allocator allocator {};
        Json::Mbs::Document originalJsonDoc { &allocator };
        Json::Mbs::Document loadedJsonDoc { &allocator };
        std::filesystem::path jsonFile { tempFile() };

        std::string original { jsonObjStr };
        std::string loaded {};

        REQUIRE_NOTHROW(originalJsonDoc <<= original);
        REQUIRE_NOTHROW(originalJsonDoc >>= jsonFile);
        REQUIRE_NOTHROW(loadedJsonDoc <<= jsonFile);
        REQUIRE_NOTHROW(loadedJsonDoc >>= loaded);

        REQUIRE(removeWhiteSpace(std::move(original)) == removeWhiteSpace(std::move(loaded)));
        REQUIRE(originalJsonDoc == loadedJsonDoc);

        REQUIRE(loadedJsonDoc.HasMember(nullKey));
        REQUIRE(loadedJsonDoc[kv(nullKey)].IsNull());
        REQUIRE(loadedJsonDoc.RemoveMember(nullKey));

        REQUIRE(loadedJsonDoc.HasMember(emptyObjectKey));
        REQUIRE(loadedJsonDoc[kv(emptyObjectKey)].IsObject());
        REQUIRE(loadedJsonDoc[kv(emptyObjectKey)].ObjectEmpty());
        REQUIRE(loadedJsonDoc.RemoveMember(emptyObjectKey));

        REQUIRE(loadedJsonDoc.HasMember(emptyArrayKey));
        REQUIRE(loadedJsonDoc[kv(emptyArrayKey)].IsArray());
        REQUIRE(loadedJsonDoc[kv(emptyArrayKey)].Empty());
        REQUIRE(loadedJsonDoc.RemoveMember(emptyArrayKey));

        REQUIRE(loadedJsonDoc.HasMember(arrayKey));
        REQUIRE(loadedJsonDoc[kv(arrayKey)].IsArray());
        REQUIRE(loadedJsonDoc[kv(arrayKey)].Size() == 5);
        for (SizeType i {}; i < 5; ++i) {
            REQUIRE(loadedJsonDoc[kv(arrayKey)][i].IsInt());
            REQUIRE(loadedJsonDoc[kv(arrayKey)][i].GetInt() == static_cast<int>(i + 1));
        }
        REQUIRE(loadedJsonDoc.RemoveMember(arrayKey));

        REQUIRE(loadedJsonDoc.HasMember(trueKey));
        REQUIRE(loadedJsonDoc[kv(trueKey)].IsBool());
        REQUIRE(loadedJsonDoc[kv(trueKey)].GetBool() == trueVal);
        REQUIRE(loadedJsonDoc.RemoveMember(trueKey));

        REQUIRE(loadedJsonDoc.HasMember(trueStr1Key));
        REQUIRE(loadedJsonDoc[kv(trueStr1Key)].IsString());
        REQUIRE(loadedJsonDoc[kv(trueStr1Key)].GetString() == trueStr1Val);
        REQUIRE(loadedJsonDoc.RemoveMember(trueStr1Key));

        REQUIRE(loadedJsonDoc.HasMember(trueStr2Key));
        REQUIRE(loadedJsonDoc[kv(trueStr2Key)].IsString());
        REQUIRE(loadedJsonDoc[kv(trueStr2Key)].GetString() == trueStr2Val);
        REQUIRE(loadedJsonDoc.RemoveMember(trueStr2Key));

        REQUIRE(loadedJsonDoc.HasMember(trueStr3Key));
        REQUIRE(loadedJsonDoc[kv(trueStr3Key)].IsString());
        REQUIRE(loadedJsonDoc[kv(trueStr3Key)].GetString() == trueStr3Val);
        REQUIRE(loadedJsonDoc.RemoveMember(trueStr3Key));

        REQUIRE(loadedJsonDoc.HasMember(falseKey));
        REQUIRE(loadedJsonDoc[kv(falseKey)].IsBool());
        REQUIRE(loadedJsonDoc[kv(falseKey)].GetBool() == falseVal);
        REQUIRE(loadedJsonDoc.RemoveMember(falseKey));

        REQUIRE(loadedJsonDoc.HasMember(falseStr1Key));
        REQUIRE(loadedJsonDoc[kv(falseStr1Key)].IsString());
        REQUIRE(loadedJsonDoc[kv(falseStr1Key)].GetString() == falseStr1Val);
        REQUIRE(loadedJsonDoc.RemoveMember(falseStr1Key));

        REQUIRE(loadedJsonDoc.HasMember(falseStr2Key));
        REQUIRE(loadedJsonDoc[kv(falseStr2Key)].IsString());
        REQUIRE(loadedJsonDoc[kv(falseStr2Key)].GetString() == falseStr2Val);
        REQUIRE(loadedJsonDoc.RemoveMember(falseStr2Key));

        REQUIRE(loadedJsonDoc.HasMember(falseStr3Key));
        REQUIRE(loadedJsonDoc[kv(falseStr3Key)].IsString());
        REQUIRE(loadedJsonDoc[kv(falseStr3Key)].GetString() == falseStr3Val);
        REQUIRE(loadedJsonDoc.RemoveMember(falseStr3Key));

        REQUIRE(loadedJsonDoc.HasMember(zeroNumKey));
        REQUIRE(loadedJsonDoc[kv(zeroNumKey)].IsInt());
        REQUIRE(loadedJsonDoc[kv(zeroNumKey)].GetInt() == zeroNumVal);
        REQUIRE(loadedJsonDoc.RemoveMember(zeroNumKey));

        REQUIRE(loadedJsonDoc.HasMember(zeroStrKey));
        REQUIRE(loadedJsonDoc[kv(zeroStrKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(zeroStrKey)].GetString() == zeroNumStrVal);
        REQUIRE(loadedJsonDoc.RemoveMember(zeroStrKey));

        REQUIRE(loadedJsonDoc.HasMember(fiveNumKey));
        REQUIRE(loadedJsonDoc[kv(fiveNumKey)].IsInt());
        REQUIRE(loadedJsonDoc[kv(fiveNumKey)].GetInt() == fiveNumVal);
        REQUIRE(loadedJsonDoc.RemoveMember(fiveNumKey));

        REQUIRE(loadedJsonDoc.HasMember(sevenStrKey));
        REQUIRE(loadedJsonDoc[kv(sevenStrKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(sevenStrKey)].GetString() == sevenStrVal);
        REQUIRE(loadedJsonDoc.RemoveMember(sevenStrKey));

        REQUIRE(loadedJsonDoc.HasMember(pintNumKey));
        REQUIRE(loadedJsonDoc[kv(pintNumKey)].IsInt());
        REQUIRE(loadedJsonDoc[kv(pintNumKey)].GetInt() == pintNumVal);
        REQUIRE(loadedJsonDoc.RemoveMember(pintNumKey));

        REQUIRE(loadedJsonDoc.HasMember(pintStrKey));
        REQUIRE(loadedJsonDoc[kv(pintStrKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(pintStrKey)].GetString() == pintStrVal);
        REQUIRE(loadedJsonDoc.RemoveMember(pintStrKey));

        REQUIRE(loadedJsonDoc.HasMember(nintNumKey));
        REQUIRE(loadedJsonDoc[kv(nintNumKey)].IsInt());
        REQUIRE(loadedJsonDoc[kv(nintNumKey)].GetInt() == nintNumVal);
        REQUIRE(loadedJsonDoc.RemoveMember(nintNumKey));

        REQUIRE(loadedJsonDoc.HasMember(nintStrKey));
        REQUIRE(loadedJsonDoc[kv(nintStrKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(nintStrKey)].GetString() == nintStrVal);
        REQUIRE(loadedJsonDoc.RemoveMember(nintStrKey));

        REQUIRE(loadedJsonDoc.HasMember(pdblNumKey));
        REQUIRE(loadedJsonDoc[kv(pdblNumKey)].IsDouble());
        REQUIRE_THAT(loadedJsonDoc[kv(pdblNumKey)].GetDouble(), WithinRel(pdblNumVal, 0.000001));
        REQUIRE(loadedJsonDoc.RemoveMember(pdblNumKey));

        REQUIRE(loadedJsonDoc.HasMember(pdblStrKey));
        REQUIRE(loadedJsonDoc[kv(pdblStrKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(pdblStrKey)].GetString() == pdblStrVal);
        REQUIRE(loadedJsonDoc.RemoveMember(pdblStrKey));

        REQUIRE(loadedJsonDoc.HasMember(ndblNumKey));
        REQUIRE(loadedJsonDoc[kv(ndblNumKey)].IsDouble());
        REQUIRE_THAT(loadedJsonDoc[kv(ndblNumKey)].GetDouble(), WithinRel(ndblNumVal, 0.000001));
        REQUIRE(loadedJsonDoc.RemoveMember(ndblNumKey));

        REQUIRE(loadedJsonDoc.HasMember(ndblStrKey));
        REQUIRE(loadedJsonDoc[kv(ndblStrKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(ndblStrKey)].GetString() == ndblStrVal);
        REQUIRE(loadedJsonDoc.RemoveMember(ndblStrKey));

        REQUIRE(loadedJsonDoc.HasMember(strKey));
        REQUIRE(loadedJsonDoc[kv(strKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(strKey)].GetString() == strVal);
        REQUIRE(loadedJsonDoc.RemoveMember(strKey));

        REQUIRE(loadedJsonDoc.HasMember(subKey));
        REQUIRE(loadedJsonDoc[kv(subKey)].IsObject());

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(nullKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(nullKey)].IsNull());
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(nullKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(emptyObjectKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(emptyObjectKey)].IsObject());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(emptyObjectKey)].ObjectEmpty());
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(emptyObjectKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(emptyArrayKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(emptyArrayKey)].IsArray());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(emptyArrayKey)].Empty());
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(emptyArrayKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(arrayKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(arrayKey)].IsArray());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(arrayKey)].Size() == 5);
        for (SizeType i {}; i < 5; ++i) {
            REQUIRE(loadedJsonDoc[kv(subKey)][kv(arrayKey)][i].IsInt());
            REQUIRE(loadedJsonDoc[kv(subKey)][kv(arrayKey)][i].GetInt() == static_cast<int>(i + 1));
        }
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(arrayKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(trueKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(trueKey)].IsBool());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(trueKey)].GetBool() == trueVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(trueKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(trueStr1Key));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(trueStr1Key)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(trueStr1Key)].GetString() == trueStr1Val);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(trueStr1Key));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(trueStr2Key));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(trueStr2Key)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(trueStr2Key)].GetString() == trueStr2Val);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(trueStr2Key));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(trueStr3Key));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(trueStr3Key)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(trueStr3Key)].GetString() == trueStr3Val);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(trueStr3Key));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(falseKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(falseKey)].IsBool());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(falseKey)].GetBool() == falseVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(falseKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(falseStr1Key));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(falseStr1Key)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(falseStr1Key)].GetString() == falseStr1Val);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(falseStr1Key));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(falseStr2Key));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(falseStr2Key)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(falseStr2Key)].GetString() == falseStr2Val);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(falseStr2Key));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(falseStr3Key));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(falseStr3Key)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(falseStr3Key)].GetString() == falseStr3Val);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(falseStr3Key));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(zeroNumKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(zeroNumKey)].IsInt());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(zeroNumKey)].GetInt() == zeroNumVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(zeroNumKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(zeroStrKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(zeroStrKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(zeroStrKey)].GetString() == zeroNumStrVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(zeroStrKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(fiveNumKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(fiveNumKey)].IsInt());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(fiveNumKey)].GetInt() == fiveNumVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(fiveNumKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(sevenStrKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(sevenStrKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(sevenStrKey)].GetString() == sevenStrVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(sevenStrKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(pintNumKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(pintNumKey)].IsInt());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(pintNumKey)].GetInt() == pintNumVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(pintNumKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(pintStrKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(pintStrKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(pintStrKey)].GetString() == pintStrVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(pintStrKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(nintNumKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(nintNumKey)].IsInt());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(nintNumKey)].GetInt() == nintNumVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(nintNumKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(nintStrKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(nintStrKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(nintStrKey)].GetString() == nintStrVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(nintStrKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(pdblNumKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(pdblNumKey)].IsDouble());
        REQUIRE_THAT(loadedJsonDoc[kv(subKey)][kv(pdblNumKey)].GetDouble(), WithinRel(pdblNumVal, 0.000001));
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(pdblNumKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(pdblStrKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(pdblStrKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(pdblStrKey)].GetString() == pdblStrVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(pdblStrKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(ndblNumKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(ndblNumKey)].IsDouble());
        REQUIRE_THAT(loadedJsonDoc[kv(subKey)][kv(ndblNumKey)].GetDouble(), WithinRel(ndblNumVal, 0.000001));
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(ndblNumKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(ndblStrKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(ndblStrKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(ndblStrKey)].GetString() == ndblStrVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(ndblStrKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].HasMember(strKey));
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(strKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(subKey)][kv(strKey)].GetString() == strVal);
        REQUIRE(loadedJsonDoc[kv(subKey)].RemoveMember(strKey));

        REQUIRE(loadedJsonDoc[kv(subKey)].ObjectEmpty());
        REQUIRE(loadedJsonDoc.RemoveMember(subKey));
        REQUIRE(loadedJsonDoc.ObjectEmpty());
    }

    TEST_CASE("Check encoding of content written to file (MBS JSON object)", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document originalJsonDoc { Json::Type::Object, &allocator };
        Json::Mbs::Document loadedJsonDoc { &allocator };
        std::filesystem::path jsonFile { tempFile() };

        originalJsonDoc.AddMember(encodingTestKey, Json::Mbs::Value { encodingTestVal.c_str(), allocator }, allocator);

        REQUIRE_NOTHROW(originalJsonDoc >>= jsonFile);
        REQUIRE_NOTHROW(loadedJsonDoc <<= jsonFile);

        REQUIRE(loadedJsonDoc.IsObject());
        REQUIRE(loadedJsonDoc.HasMember(encodingTestKey));
        REQUIRE(loadedJsonDoc[kv(encodingTestKey)].IsString());
        REQUIRE(loadedJsonDoc[kv(encodingTestKey)].GetString() == encodingTestVal);
        REQUIRE(loadedJsonDoc.RemoveMember(encodingTestKey));
        REQUIRE(loadedJsonDoc.ObjectEmpty());

        std::ifstream file { jsonFile };

        REQUIRE(file.is_open());

        std::string written { (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>() };
        std::string original { };

        REQUIRE_NOTHROW(originalJsonDoc >>= original);
        REQUIRE(removeWhiteSpace(std::move(written)) == removeWhiteSpace(std::move(original)));

        file.close();
    }
}
