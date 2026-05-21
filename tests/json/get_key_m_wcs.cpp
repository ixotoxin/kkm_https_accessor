// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/xetters.h>
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Getting WCS JSON object property with cast map #1", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nullKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyObjectKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyArrayKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, arrayKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr1Key, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr2Key, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr3Key, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr1Key, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr2Key, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr3Key, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroNumKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroStrKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, fiveNumKey, output, i2sCastMap));
            REQUIRE(output == fiveStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, sevenStrKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintNumKey, output, i2sCastMap));
            REQUIRE(output == cookieVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintStrKey, output, i2sCastMap));
            REQUIRE(output == cookieVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintNumKey, output, i2sCastMap));
            REQUIRE(output == coalVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintStrKey, output, i2sCastMap));
            REQUIRE(output == coalVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblNumKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblStrKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblNumKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblStrKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, strKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, subKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nepKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }
    }

    TEST_CASE("Getting WCS JSON object property with cast map #2", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nullKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyObjectKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyArrayKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, arrayKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueKey, output, s2vCastMap));
            REQUIRE(output == trueStr1Val);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr1Key, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueStr2Key, output, s2vCastMap));
            REQUIRE(output == trueStr1Val);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr3Key, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseKey, output, s2vCastMap));
            REQUIRE(output == falseStr1Val);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr1Key, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseStr2Key, output, s2vCastMap));
            REQUIRE(output == falseStr1Val);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr3Key, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroNumKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroStrKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, fiveNumKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, sevenStrKey, output, s2vCastMap));
            REQUIRE(output == sevenNumStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintNumKey, output, s2vCastMap));
            REQUIRE(output == cookieVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintStrKey, output, s2vCastMap));
            REQUIRE(output == cookieVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintNumKey, output, s2vCastMap));
            REQUIRE(output == coalVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintStrKey, output, s2vCastMap));
            REQUIRE(output == coalVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblNumKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblStrKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblNumKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblStrKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, strKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, subKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nepKey, output, s2vCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }
    }

    TEST_CASE("Getting WCS JSON object property with cast map #3", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nullKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyObjectKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyArrayKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, arrayKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueKey, output, v2iCastMap));
            REQUIRE(output == oneNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr1Key, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueStr2Key, output, v2iCastMap));
            REQUIRE(output == oneNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr3Key, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseKey, output, v2iCastMap));
            REQUIRE(output == zeroNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr1Key, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseStr2Key, output, v2iCastMap));
            REQUIRE(output == zeroNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr3Key, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroNumKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroStrKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, fiveNumKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, sevenStrKey, output, v2iCastMap));
            REQUIRE(output == sevenNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintNumKey, output, v2iCastMap));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintStrKey, output, v2iCastMap));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintNumKey, output, v2iCastMap));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintStrKey, output, v2iCastMap));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblNumKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblStrKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblNumKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblStrKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, strKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, subKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nepKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }
    }
}
