// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/inout.h>
#include <json/cast.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Cast WCS JSON value with cast map #1", "[json][wcs]") {
        using Catch::Matchers::WithinRel;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(fiveNumKey)], i2dCastMap), WithinRel(55.55, 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(pintNumKey)], i2dCastMap), WithinRel(pdblNumVal, 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(pintStrKey)], i2dCastMap), WithinRel(pdblNumVal, 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(nintNumKey)], i2dCastMap), WithinRel(ndblNumVal, 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(nintStrKey)], i2dCastMap), WithinRel(ndblNumVal, 0.000001));

        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(nullKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(emptyObjectKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(emptyArrayKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(arrayKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(trueKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(trueStr1Key)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(trueStr2Key)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(trueStr3Key)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(falseKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(falseStr1Key)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(falseStr2Key)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(falseStr3Key)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(zeroNumKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(zeroStrKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(sevenStrKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(pdblNumKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(pdblStrKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(ndblNumKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(ndblStrKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(strKey)], i2dCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(subKey)], i2dCastMap), Basic::DataError);
    }

    TEST_CASE("Cast WCS JSON value with cast map #2", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(fiveNumKey)], i2sCastMap) == fiveStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(pintNumKey)], i2sCastMap) == cookieVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(pintStrKey)], i2sCastMap) == cookieVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(nintNumKey)], i2sCastMap) == coalVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(nintStrKey)], i2sCastMap) == coalVal);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(nullKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(emptyObjectKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(emptyArrayKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(arrayKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(trueKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(trueStr1Key)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(trueStr2Key)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(trueStr3Key)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(falseKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(falseStr1Key)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(falseStr2Key)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(falseStr3Key)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(zeroNumKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(zeroStrKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(sevenStrKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(pdblNumKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(pdblStrKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(ndblNumKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(ndblStrKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(strKey)], i2sCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(subKey)], i2sCastMap), Basic::DataError);
    }

    TEST_CASE("Cast WCS JSON value with cast map #3", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(Json::cast<short>(jsonDoc[kv(trueKey)], v2iCastMap) == oneNumVal);
        REQUIRE(Json::cast<short>(jsonDoc[kv(trueStr2Key)], v2iCastMap) == oneNumVal);
        REQUIRE(Json::cast<short>(jsonDoc[kv(falseKey)], v2iCastMap) == zeroNumVal);
        REQUIRE(Json::cast<short>(jsonDoc[kv(falseStr2Key)], v2iCastMap) == zeroNumVal);
        REQUIRE(Json::cast<short>(jsonDoc[kv(sevenStrKey)], v2iCastMap) == sevenNumVal);
        REQUIRE(Json::cast<short>(jsonDoc[kv(pintNumKey)], v2iCastMap) == nintNumVal);
        REQUIRE(Json::cast<short>(jsonDoc[kv(pintStrKey)], v2iCastMap) == nintNumVal);
        REQUIRE(Json::cast<short>(jsonDoc[kv(nintNumKey)], v2iCastMap) == pintNumVal);
        REQUIRE(Json::cast<short>(jsonDoc[kv(nintStrKey)], v2iCastMap) == pintNumVal);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(emptyObjectKey)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(emptyArrayKey)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(arrayKey)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(nullKey)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(trueStr1Key)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(trueStr3Key)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(falseStr1Key)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(falseStr3Key)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(zeroNumKey)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(zeroStrKey)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(fiveNumKey)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(pdblNumKey)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(pdblStrKey)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(ndblNumKey)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(ndblStrKey)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(strKey)], v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<short>(jsonDoc[kv(subKey)], v2iCastMap), Basic::DataError);
    }

    TEST_CASE("Cast WCS JSON value with cast map #4", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(trueKey)], s2vCastMap) == trueStr1Val);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(trueStr2Key)], s2vCastMap) == trueStr1Val);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(falseKey)], s2vCastMap) == falseStr1Val);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(falseStr2Key)], s2vCastMap) == falseStr1Val);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(sevenStrKey)], s2vCastMap) == sevenNumStrVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(pintNumKey)], s2vCastMap) == cookieVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(pintStrKey)], s2vCastMap) == cookieVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(nintNumKey)], s2vCastMap) == coalVal);
        REQUIRE(Json::cast<std::wstring>(jsonDoc[kv(nintStrKey)], s2vCastMap) == coalVal);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(emptyObjectKey)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(emptyArrayKey)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(arrayKey)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(nullKey)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(trueStr1Key)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(trueStr3Key)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(falseStr1Key)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(falseStr3Key)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(zeroNumKey)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(zeroStrKey)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(fiveNumKey)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(pdblNumKey)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(pdblStrKey)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(ndblNumKey)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(ndblStrKey)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(strKey)], s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::wstring>(jsonDoc[kv(subKey)], s2vCastMap), Basic::DataError);
    }
}
