// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/inout.h>
#include <json/cast.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Cast WCS JSON value to std::wstring", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(nullKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(emptyObjectKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(emptyArrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(arrayKey)]), Basic::DataError);

        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(trueKey)]) == Text::yesNo<Meta::Wcs>(trueVal));
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(trueStr1Key)]) == trueStr1Val);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(trueStr2Key)]) == trueStr2Val);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(trueStr3Key)]) == trueStr3Val);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(falseKey)]) == Text::yesNo<Meta::Wcs>(falseVal));
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(falseStr1Key)]) == falseStr1Val);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(falseStr2Key)]) == falseStr2Val);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(falseStr3Key)]) == falseStr3Val);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(zeroNumKey)]) == zeroNumStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(zeroStrKey)]) == zeroNumStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(pintNumKey)]) == pintStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(pintStrKey)]) == pintStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(nintNumKey)]) == nintStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(nintStrKey)]) == nintStrVal);

        {
            std::wstring testValue { strCheckVal };
            REQUIRE_NOTHROW(testValue = Json::cast<std::wstring>(jsonDoc[kv(pdblNumKey)]));
            REQUIRE(removeTrailingZeros(testValue) == pdblStrVal);
        }

        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(pdblStrKey)]) == pdblStrVal);

        {
            std::wstring testValue { strCheckVal };
            REQUIRE_NOTHROW(testValue = Json::cast<std::wstring>(jsonDoc[kv(ndblNumKey)]));
            REQUIRE(removeTrailingZeros(testValue) == ndblStrVal);
        }

        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(ndblStrKey)]) == ndblStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(strKey)]) == strVal);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(subKey)]), Basic::DataError);
    }

    TEST_CASE("Cast WCS JSON value to std::filesystem::path", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(trueStr1Key)]) == trueStr1Val);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(trueStr2Key)]) == trueStr2Val);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(trueStr3Key)]) == trueStr3Val);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(falseStr1Key)]) == falseStr1Val);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(falseStr2Key)]) == falseStr2Val);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(falseStr3Key)]) == falseStr3Val);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(zeroStrKey)]) == zeroNumStrVal);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(pintStrKey)]) == pintStrVal);
        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(nintStrKey)]) == nintStrVal);

        {
            std::filesystem::path testValue {};
            REQUIRE_NOTHROW(testValue = Json::cast<std::filesystem::path>(jsonDoc[kv(pdblStrKey)]));
            REQUIRE(removeTrailingZeros(testValue.wstring()) == pdblStrVal);
        }

        {
            std::filesystem::path testValue {};
            REQUIRE_NOTHROW(testValue = Json::cast<std::filesystem::path>(jsonDoc[kv(ndblStrKey)]));
            REQUIRE(removeTrailingZeros(testValue.wstring()) == ndblStrVal);
        }

        REQUIRE(Json::cast<std::filesystem::path>(jsonDoc[kv(strKey)]) == strVal);

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
