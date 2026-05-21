// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include "test_data_mbs.h"
#include <json/inout.h>
#include <json/cast.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Cast MBS JSON value to std::string", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::string>(jsonDoc[kv(nullKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::string>(jsonDoc[kv(emptyObjectKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::string>(jsonDoc[kv(emptyArrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::string>(jsonDoc[kv(arrayKey)]), Basic::DataError);

        REQUIRE(Json::cast<std::string>(jsonDoc[kv(trueKey)]) == Text::yesNo<Meta::Mbs>(trueVal));
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(trueStr1Key)]) == trueStr1Val);
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(trueStr2Key)]) == trueStr2Val);
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(trueStr3Key)]) == trueStr3Val);
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(falseKey)]) == Text::yesNo<Meta::Mbs>(falseVal));
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(falseStr1Key)]) == falseStr1Val);
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(falseStr2Key)]) == falseStr2Val);
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(falseStr3Key)]) == falseStr3Val);
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(zeroNumKey)]) == zeroNumStrVal);
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(zeroStrKey)]) == zeroNumStrVal);
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(pintNumKey)]) == pintStrVal);
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(pintStrKey)]) == pintStrVal);
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(nintNumKey)]) == nintStrVal);
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(nintStrKey)]) == nintStrVal);

        {
            std::string testValue { strCheckVal };
            REQUIRE_NOTHROW(testValue = Json::cast<std::string>(jsonDoc[kv(pdblNumKey)]));
            REQUIRE(removeTrailingZeros(testValue) == pdblStrVal);
        }

        REQUIRE(Json::cast<std::string>(jsonDoc[kv(pdblStrKey)]) == pdblStrVal);

        {
            std::string testValue { strCheckVal };
            REQUIRE_NOTHROW(testValue = Json::cast<std::string>(jsonDoc[kv(ndblNumKey)]));
            REQUIRE(removeTrailingZeros(testValue) == ndblStrVal);
        }

        REQUIRE(Json::cast<std::string>(jsonDoc[kv(ndblStrKey)]) == ndblStrVal);
        REQUIRE(Json::cast<std::string>(jsonDoc[kv(strKey)]) == strVal);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::string>(jsonDoc[kv(subKey)]), Basic::DataError);
    }

    TEST_CASE("Cast MBS JSON value to std::filesystem::path", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(trueStr1Key)]) == Wcs::trueStr1Val);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(trueStr2Key)]) == Wcs::trueStr2Val);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(trueStr3Key)]) == Wcs::trueStr3Val);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(falseStr1Key)]) == Wcs::falseStr1Val);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(falseStr2Key)]) == Wcs::falseStr2Val);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(falseStr3Key)]) == Wcs::falseStr3Val);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(zeroStrKey)]) == Wcs::zeroNumStrVal);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(pintStrKey)]) == Wcs::pintStrVal);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(nintStrKey)]) == Wcs::nintStrVal);

        {
            std::filesystem::path testValue {};
            REQUIRE_NOTHROW(testValue = Json::cast<std::filesystem::path>(jsonDoc[kv(pdblStrKey)]));
            REQUIRE(Wcs::removeTrailingZeros(testValue.wstring()) == Wcs::pdblStrVal);
        }

        {
            std::filesystem::path testValue {};
            REQUIRE_NOTHROW(testValue = Json::cast<std::filesystem::path>(jsonDoc[kv(ndblStrKey)]));
            REQUIRE(Wcs::removeTrailingZeros(testValue.wstring()) == Wcs::ndblStrVal);
        }

        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(strKey)]) == Wcs::strVal);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::filesystem::path>(jsonDoc[kv(nullKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::filesystem::path>(jsonDoc[kv(trueKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::filesystem::path>(jsonDoc[kv(falseKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::filesystem::path>(jsonDoc[kv(zeroNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::filesystem::path>(jsonDoc[kv(pintNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::filesystem::path>(jsonDoc[kv(nintNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::filesystem::path>(jsonDoc[kv(pdblNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::filesystem::path>(jsonDoc[kv(ndblNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::filesystem::path>(jsonDoc[kv(emptyObjectKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::filesystem::path>(jsonDoc[kv(emptyArrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::filesystem::path>(jsonDoc[kv(arrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::filesystem::path>(jsonDoc[kv(subKey)]), Basic::DataError);
    }
}
