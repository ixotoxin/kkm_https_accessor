// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/inout.h>
#include <json/cast.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Cast WCS JSON value to integer within domain", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(Json::cast<short>(jsonDoc[kv(fiveNumKey)], intDomain) == fiveNumVal);
        REQUIRE(Json::cast<short>(jsonDoc[kv(pintNumKey)], intDomain) == pintNumVal);
        REQUIRE(Json::cast<short>(jsonDoc[kv(pintStrKey)], intDomain) == pintNumVal);
        REQUIRE(Json::cast<short>(jsonDoc[kv(nintNumKey)], intDomain) == nintNumVal);
        REQUIRE(Json::cast<short>(jsonDoc[kv(nintStrKey)], intDomain) == nintNumVal);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(nullKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(emptyObjectKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(emptyArrayKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(arrayKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(trueKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(trueStr1Key)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(trueStr2Key)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(trueStr3Key)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(falseKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(falseStr1Key)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(falseStr2Key)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(falseStr3Key)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(zeroNumKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(zeroStrKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(sevenStrKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(pdblNumKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(pdblStrKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(ndblNumKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(ndblStrKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(strKey)], intDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(subKey)], intDomain), Basic::DataError);
    }

    TEST_CASE("Cast WCS JSON value to std::wstring within domain #1", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(trueKey)], strDomain) == Text::yesNo<Meta::Wcs>(trueVal));
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(trueStr2Key)], strDomain) == trueStr2Val);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(pintNumKey)], strDomain) == pintStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(pintStrKey)], strDomain) == pintStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(nintNumKey)], strDomain) == nintStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(nintStrKey)], strDomain) == nintStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(strKey)], strDomain) == loweredStrVal);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(emptyObjectKey)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(emptyArrayKey)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(arrayKey)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(nullKey)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(trueStr1Key)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(trueStr3Key)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(falseKey)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(falseStr1Key)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(falseStr2Key)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(falseStr3Key)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(zeroNumKey)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(zeroStrKey)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(pdblNumKey)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(pdblStrKey)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(ndblNumKey)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(ndblStrKey)], strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(subKey)], strDomain), Basic::DataError);
    }

    TEST_CASE("Cast WCS JSON value to std::wstring within domain #2", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(falseKey)], svwDomain) == Text::yesNo<Meta::Wcs>(falseVal));
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(falseStr2Key)], svwDomain) == falseStr2Val);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(pintNumKey)], strDomain) == pintStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(pintStrKey)], strDomain) == pintStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(nintNumKey)], strDomain) == nintStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(nintStrKey)], strDomain) == nintStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(strKey)], svwDomain) == loweredStrVal);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(emptyObjectKey)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(emptyArrayKey)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(arrayKey)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(nullKey)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(trueKey)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(trueStr1Key)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(trueStr2Key)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(trueStr3Key)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(falseStr1Key)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(falseStr3Key)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(zeroNumKey)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(zeroStrKey)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(pdblNumKey)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(pdblStrKey)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(ndblNumKey)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(ndblStrKey)], svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(subKey)], svwDomain), Basic::DataError);
    }
}
