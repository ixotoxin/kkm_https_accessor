// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/xetters.h>
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Handle MBS JSON value or object property with cast map #1", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output, i2sCastMap));
            REQUIRE(output == cookieVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintStrKey)], output, i2sCastMap));
            REQUIRE(output == cookieVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(nintNumKey)], output, i2sCastMap));
            REQUIRE(output == coalVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(nintStrKey)], output, i2sCastMap));
            REQUIRE(output == coalVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handle(jsonDoc[kv(nullKey)], output, i2sCastMap));
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(zeroNumKey)], output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(zeroStrKey)], output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(pdblNumKey)], output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(pdblStrKey)], output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(ndblNumKey)], output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(ndblStrKey)], output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(strKey)], output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property with cast map #2", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintNumKey, output, i2sCastMap));
            REQUIRE(output == cookieVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintStrKey, output, i2sCastMap));
            REQUIRE(output == cookieVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, nintNumKey, output, i2sCastMap));
            REQUIRE(output == coalVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, nintStrKey, output, i2sCastMap));
            REQUIRE(output == coalVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nullKey, output, i2sCastMap));
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nepKey, output, i2sCastMap));
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, zeroNumKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, zeroStrKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, pdblNumKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, pdblStrKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, ndblNumKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, ndblStrKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, strKey, output, i2sCastMap), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property with cast map #3", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::string output { strCheckVal };
            REQUIRE(
                Json::handle(
                    jsonDoc[kv(fiveNumKey)], output, i2sCastMap,
                    [] (const std::string & v) { return "~"s + v + "~"s; }
                )
            );
            REQUIRE(output == "~"s + fiveStrVal + "~"s);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output, i2sCastMap, strAddTilda));
            REQUIRE(output == "~"s + std::string { cookieVal } + "~"s);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handle(
                    jsonDoc[kv(pintStrKey)], output, i2sCastMap,
                    Text::Mbs::maxLength(pintStrVal.size() - 1)
                ),
                Basic::DataError
            );
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(nintNumKey)], output, i2sCastMap, Text::Mbs::lower()));
            REQUIRE_FALSE(output == coalVal);
            REQUIRE(output == loweredCoalVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handle(jsonDoc[kv(nullKey)], output, i2sCastMap, strThrow));
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(
                Json::handleKey(
                    jsonDoc, fiveNumKey, output, i2sCastMap,
                    [] (const std::string & v) { return "~"s + v + "~"s; }
                )
            );
            REQUIRE(output == "~"s + fiveStrVal + "~"s);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintNumKey, output, i2sCastMap, strAddTilda));
            REQUIRE(output == "~"s + std::string { cookieVal } + "~"s);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handleKey(
                    jsonDoc, pintStrKey, output, i2sCastMap,
                    Text::Mbs::maxLength(pintStrVal.size() - 1)
                ),
                Basic::DataError
            );
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, nintNumKey, output, i2sCastMap, strAddTilda));
            REQUIRE_FALSE(output == coalVal);
            REQUIRE(output == "~"s + std::string { coalVal } + "~"s);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nullKey, output, i2sCastMap, strThrow));
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nepKey, output, i2sCastMap, Text::Mbs::lower()));
            REQUIRE(output == strCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property with cast map #4", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output, v2iCastMap));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintStrKey)], output, v2iCastMap));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(nintNumKey)], output, v2iCastMap));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(nintStrKey)], output, v2iCastMap));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handle(jsonDoc[kv(nullKey)], output, v2iCastMap));
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(zeroNumKey)], output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(zeroStrKey)], output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(pdblNumKey)], output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(pdblStrKey)], output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(ndblNumKey)], output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(ndblStrKey)], output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(strKey)], output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property with cast map #5", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintNumKey, output, v2iCastMap));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintStrKey, output, v2iCastMap));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, nintNumKey, output, v2iCastMap));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, nintStrKey, output, v2iCastMap));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nullKey, output, v2iCastMap));
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nepKey, output, v2iCastMap));
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, zeroNumKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, zeroStrKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, pdblNumKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, pdblStrKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, ndblNumKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, ndblStrKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, strKey, output, v2iCastMap), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property with cast map #6", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output, v2iCastMap, [] (const int v) { return v + 10; }));
            REQUIRE(output == nintNumVal + 10);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintStrKey)], output, v2iCastMap, Numeric::clamp(nintNumVal + 10, pintNumVal)));
            REQUIRE(output == nintNumVal + 10);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output, v2iCastMap, intAdd10));
            REQUIRE(output == nintNumVal + 10);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handle(jsonDoc[kv(pintStrKey)], output, v2iCastMap, Numeric::min(nintNumVal + 10)),
                Basic::DataError
            );
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handle(jsonDoc[kv(nullKey)], output, v2iCastMap, intThrow));
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintNumKey, output, v2iCastMap, [] (const int v) { return v + 10; }));
            REQUIRE(output == nintNumVal + 10);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintStrKey, output, v2iCastMap, Numeric::clamp(nintNumVal + 10, pintNumVal)));
            REQUIRE(output == nintNumVal + 10);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handleKey(jsonDoc, pintNumKey, output, v2iCastMap, intThrow),
                Basic::DataError
            );
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handleKey(jsonDoc, pintStrKey, output, v2iCastMap, Numeric::min(nintNumVal + 10)),
                Basic::DataError
            );
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nepKey, output, v2iCastMap, intThrow));
            REQUIRE(output == intCheckVal);
        }
    }
}
