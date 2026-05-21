// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/inout.h>
#include <json/cast.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Cast WCS JSON value to boolean", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(Json::cast<bool>(jsonDoc[kv(trueKey)]) == trueVal);
        REQUIRE(Json::cast<bool>(jsonDoc[kv(trueStr1Key)]) == trueVal);
        REQUIRE(Json::cast<bool>(jsonDoc[kv(trueStr2Key)]) == trueVal);
        REQUIRE(Json::cast<bool>(jsonDoc[kv(trueStr3Key)]) == trueVal);
        REQUIRE(Json::cast<bool>(jsonDoc[kv(falseKey)]) == falseVal);
        REQUIRE(Json::cast<bool>(jsonDoc[kv(falseStr1Key)]) == falseVal);
        REQUIRE(Json::cast<bool>(jsonDoc[kv(falseStr2Key)]) == falseVal);
        REQUIRE(Json::cast<bool>(jsonDoc[kv(falseStr3Key)]) == falseVal);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(nullKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(emptyObjectKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(emptyArrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(arrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(zeroNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(zeroStrKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(pintNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(pintStrKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(nintNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(nintStrKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(pdblNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(ndblStrKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(ndblNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(ndblStrKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(strKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<bool>(jsonDoc[kv(subKey)]), Basic::DataError);
    }

    TEST_CASE("Cast WCS JSON value to int64", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(Json::cast<int64_t>(jsonDoc[kv(zeroNumKey)]) == zeroNumVal);
        REQUIRE(Json::cast<int64_t>(jsonDoc[kv(zeroStrKey)]) == zeroNumVal);
        REQUIRE(Json::cast<int64_t>(jsonDoc[kv(pintNumKey)]) == pintNumVal);
        REQUIRE(Json::cast<int64_t>(jsonDoc[kv(pintStrKey)]) == pintNumVal);
        REQUIRE(Json::cast<int64_t>(jsonDoc[kv(nintNumKey)]) == nintNumVal);
        REQUIRE(Json::cast<int64_t>(jsonDoc[kv(nintStrKey)]) == nintNumVal);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(nullKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(emptyObjectKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(emptyArrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(arrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(trueKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(trueStr1Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(trueStr2Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(trueStr3Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(falseKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(falseStr1Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(falseStr2Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(falseStr3Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(pdblNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(pdblStrKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(ndblNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(ndblStrKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(strKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<int64_t>(jsonDoc[kv(subKey)]), Basic::DataError);
    }

    TEST_CASE("Cast WCS JSON value to uint64", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(Json::cast<uint64_t>(jsonDoc[kv(zeroNumKey)]) == zeroNumVal);
        REQUIRE(Json::cast<uint64_t>(jsonDoc[kv(zeroStrKey)]) == zeroNumVal);
        REQUIRE(Json::cast<uint64_t>(jsonDoc[kv(pintNumKey)]) == pintNumVal);
        REQUIRE(Json::cast<uint64_t>(jsonDoc[kv(pintStrKey)]) == pintNumVal);

        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(nullKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(emptyObjectKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(emptyArrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(arrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(trueKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(trueStr1Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(trueStr2Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(trueStr3Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(falseKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(falseStr1Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(falseStr2Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(falseStr3Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(nintNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(nintStrKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(pdblNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(pdblStrKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(ndblNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(ndblStrKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(strKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<uint64_t>(jsonDoc[kv(subKey)]), Basic::DataError);
    }

    TEST_CASE("Cast WCS JSON value to double", "[json][wcs]") {
        using Catch::Matchers::WithinRel;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(zeroNumKey)]), WithinRel(zeroNumVal, 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(zeroStrKey)]), WithinRel(zeroNumVal, 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(pintNumKey)]), WithinRel(std::trunc(pdblNumVal), 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(pintStrKey)]), WithinRel(std::trunc(pdblNumVal), 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(nintNumKey)]), WithinRel(std::trunc(ndblNumVal), 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(nintStrKey)]), WithinRel(std::trunc(ndblNumVal), 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(pdblNumKey)]), WithinRel(pdblNumVal, 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(pdblStrKey)]), WithinRel(pdblNumVal, 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(ndblNumKey)]), WithinRel(ndblNumVal, 0.000001));
        REQUIRE_THAT(Json::cast<double>(jsonDoc[kv(ndblStrKey)]), WithinRel(ndblNumVal, 0.000001));

        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(nullKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(emptyObjectKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(emptyArrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(arrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(trueKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(trueStr1Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(trueStr2Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(trueStr3Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(falseKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(falseStr1Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(falseStr2Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(falseStr3Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(strKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<double>(jsonDoc[kv(subKey)]), Basic::DataError);
    }

    TEST_CASE("Cast WCS JSON value to std::duration", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(Json::cast<std::chrono::milliseconds>(jsonDoc[kv(zeroNumKey)]) == std::chrono::milliseconds(zeroNumVal));
        REQUIRE(Json::cast<std::chrono::milliseconds>(jsonDoc[kv(zeroStrKey)]) == std::chrono::milliseconds(zeroNumVal));
        REQUIRE(Json::cast<std::chrono::milliseconds>(jsonDoc[kv(pintNumKey)]) == std::chrono::milliseconds(pintNumVal));
        REQUIRE(Json::cast<std::chrono::milliseconds>(jsonDoc[kv(pintNumKey)]) == std::chrono::milliseconds(pintNumVal));
        REQUIRE(Json::cast<std::chrono::milliseconds>(jsonDoc[kv(nintNumKey)]) == std::chrono::milliseconds(nintNumVal));
        REQUIRE(Json::cast<std::chrono::milliseconds>(jsonDoc[kv(nintStrKey)]) == std::chrono::milliseconds(nintNumVal));

        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(nullKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(emptyObjectKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(emptyArrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(arrayKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(trueKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(trueStr1Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(trueStr2Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(trueStr3Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(falseKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(falseStr1Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(falseStr2Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(falseStr3Key)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(pdblNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(pdblStrKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(ndblNumKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(ndblStrKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(strKey)]), Basic::DataError);
        REQUIRE_THROWS_AS(std::ignore = Json::cast<std::chrono::milliseconds>(jsonDoc[kv(subKey)]), Basic::DataError);
    }
}
