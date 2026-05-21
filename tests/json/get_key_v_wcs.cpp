// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/xetters.h>
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Getting WCS JSON object property as boolean", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            bool output { false };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nullKey, output), Basic::DataError);
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyObjectKey, output), Basic::DataError);
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyArrayKey, output), Basic::DataError);
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, arrayKey, output), Basic::DataError);
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueKey, output));
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueStr1Key, output));
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueStr2Key, output));
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueStr3Key, output));
            REQUIRE(output);
        }

        {
            bool output { true };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseKey, output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseStr1Key, output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseStr2Key, output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseStr3Key, output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroNumKey, output), Basic::DataError);
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroStrKey, output), Basic::DataError);
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, fiveNumKey, output), Basic::DataError);
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, sevenStrKey, output), Basic::DataError);
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pintNumKey, output), Basic::DataError);
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pintStrKey, output), Basic::DataError);
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nintNumKey, output), Basic::DataError);
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nintStrKey, output), Basic::DataError);
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblNumKey, output), Basic::DataError);
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblStrKey, output), Basic::DataError);
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblNumKey, output), Basic::DataError);
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblStrKey, output), Basic::DataError);
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, strKey, output), Basic::DataError);
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, subKey, output), Basic::DataError);
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nepKey, output), Basic::DataError);
            REQUIRE_FALSE(output);
        }
    }

    TEST_CASE("Getting WCS JSON object property as double", "[json][wcs]") {
        using Catch::Matchers::WithinRel;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nullKey, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyObjectKey, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyArrayKey, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, arrayKey, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueKey, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr1Key, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr2Key, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr3Key, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseKey, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr1Key, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr2Key, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr3Key, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, zeroNumKey, output));
            REQUIRE_THAT(output, WithinRel(zeroNumVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, zeroStrKey, output));
            REQUIRE_THAT(output, WithinRel(zeroNumVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, fiveNumKey, output));
            REQUIRE_THAT(output, WithinRel(fiveNumVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, sevenStrKey, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintNumKey, output));
            REQUIRE_THAT(output, WithinRel(pintNumVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintStrKey, output));
            REQUIRE_THAT(output, WithinRel(pintNumVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintNumKey, output));
            REQUIRE_THAT(output, WithinRel(nintNumVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintStrKey, output));
            REQUIRE_THAT(output, WithinRel(nintNumVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pdblNumKey, output));
            REQUIRE_THAT(output, WithinRel(pdblNumVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pdblStrKey, output));
            REQUIRE_THAT(output, WithinRel(pdblNumVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, ndblNumKey, output));
            REQUIRE_THAT(output, WithinRel(ndblNumVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, ndblStrKey, output));
            REQUIRE_THAT(output, WithinRel(ndblNumVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, strKey, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, subKey, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }

        {
            double output { dblCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nepKey, output), Basic::DataError);
            REQUIRE_THAT(output, WithinRel(dblCheckVal, 0.000001));
        }
    }

    TEST_CASE("Getting WCS JSON object property as std::wstring", "[json][wcs]") {
        using Catch::Matchers::WithinRel;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nullKey, output), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyObjectKey, output), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyArrayKey, output), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, arrayKey, output), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueKey, output));
            REQUIRE(output == Text::yesNo<Meta::Wcs>(trueVal));
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueStr1Key, output));
            REQUIRE(output == trueStr1Val);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueStr2Key, output));
            REQUIRE(output == trueStr2Val);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueStr3Key, output));
            REQUIRE(output == trueStr3Val);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseKey, output));
            REQUIRE(output == Text::yesNo<Meta::Wcs>(falseVal));
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseStr1Key, output));
            REQUIRE(output == falseStr1Val);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseStr2Key, output));
            REQUIRE(output == falseStr2Val);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseStr3Key, output));
            REQUIRE(output == falseStr3Val);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, zeroNumKey, output));
            REQUIRE(output == zeroNumStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, zeroStrKey, output));
            REQUIRE(output == zeroNumStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, fiveNumKey, output));
            REQUIRE(output == fiveNumStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, sevenStrKey, output));
            REQUIRE(output == sevenStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintNumKey, output));
            REQUIRE(output == pintStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintStrKey, output));
            REQUIRE(output == pintStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintNumKey, output));
            REQUIRE(removeTrailingZeros(output) == nintStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintStrKey, output));
            REQUIRE(output == nintStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pdblNumKey, output));
            REQUIRE(removeTrailingZeros(output) == pdblStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pdblStrKey, output));
            REQUIRE(output == pdblStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, ndblNumKey, output));
            REQUIRE(removeTrailingZeros(output) == ndblStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, ndblStrKey, output));
            REQUIRE(output == ndblStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, strKey, output));
            REQUIRE(output == strVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, subKey, output), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nepKey, output), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }
    }
}
