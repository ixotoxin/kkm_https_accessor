// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/xetters.h>
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Getting WCS JSON object property within domain #1", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nullKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyObjectKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyArrayKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, arrayKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr1Key, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr2Key, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr3Key, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr1Key, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr2Key, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr3Key, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroNumKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroStrKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, fiveNumKey, output, intDomain));
            REQUIRE(output);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, sevenStrKey, output, intDomain), Basic::DataError);
            REQUIRE(output);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintNumKey, output, intDomain));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintStrKey, output, intDomain));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintNumKey, output, intDomain));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintStrKey, output, intDomain));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblNumKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblStrKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblNumKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblStrKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, strKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, subKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nepKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }
    }

    TEST_CASE("Getting WCS JSON object property within domain #2", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nullKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyObjectKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyArrayKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, arrayKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueKey, output, strDomain));
            REQUIRE(output == Text::yesNo<Meta::Wcs>(trueVal));
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr1Key, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, trueStr2Key, output, strDomain));
            REQUIRE(output == trueStr2Val);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr3Key, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr1Key, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr2Key, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr3Key, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroNumKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroStrKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, fiveNumKey, output, strDomain));
            REQUIRE(output == fiveNumStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, sevenStrKey, output, strDomain));
            REQUIRE(output == sevenStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintNumKey, output, strDomain));
            REQUIRE(output == pintStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintStrKey, output, strDomain));
            REQUIRE(output == pintStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintNumKey, output, strDomain));
            REQUIRE(output == nintStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintStrKey, output, strDomain));
            REQUIRE(output == nintStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblNumKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblStrKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblNumKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblStrKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, strKey, output, strDomain));
            REQUIRE_FALSE(output == strVal);
            REQUIRE(output == loweredStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, subKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nepKey, output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }
    }

    TEST_CASE("Getting WCS JSON object property within domain #3", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nullKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyObjectKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, emptyArrayKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, arrayKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr1Key, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr2Key, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, trueStr3Key, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseKey, output, svwDomain));
            REQUIRE(output == Text::yesNo<Meta::Wcs>(falseVal));
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr1Key, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, falseStr2Key, output, svwDomain));
            REQUIRE(output == falseStr2Val);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, falseStr3Key, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroNumKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, zeroStrKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, fiveNumKey, output, svwDomain));
            REQUIRE(output == fiveNumStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, sevenStrKey, output, svwDomain));
            REQUIRE(output == sevenStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintNumKey, output, svwDomain));
            REQUIRE(output == pintStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, pintStrKey, output, svwDomain));
            REQUIRE(output == pintStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintNumKey, output, svwDomain));
            REQUIRE(output == nintStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, nintStrKey, output, svwDomain));
            REQUIRE(output == nintStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblNumKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, pdblStrKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblNumKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, ndblStrKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_NOTHROW(Json::getKey(jsonDoc, strKey, output, svwDomain));
            REQUIRE_FALSE(output == strVal);
            REQUIRE(output == loweredStrVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, subKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::wstring output { strCheckVal };
            REQUIRE_THROWS_AS(Json::getKey(jsonDoc, nepKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }
    }
}
