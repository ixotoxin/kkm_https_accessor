// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/xetters.h>
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Handle MBS JSON value or object property within domain #1", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output, intDomain));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintStrKey)], output, intDomain));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(nintNumKey)], output, intDomain));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(nintStrKey)], output, intDomain));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handle(jsonDoc[kv(nullKey)], output, intDomain));
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(zeroNumKey)], output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(zeroStrKey)], output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(pdblNumKey)], output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(pdblStrKey)], output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(ndblNumKey)], output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(ndblStrKey)], output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(strKey)], output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property within domain #2", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintNumKey, output, intDomain));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintStrKey, output, intDomain));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, nintNumKey, output, intDomain));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, nintStrKey, output, intDomain));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nullKey, output, intDomain));
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nepKey, output, intDomain));
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, zeroNumKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, zeroStrKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, pdblNumKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, pdblStrKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, ndblNumKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, ndblStrKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, strKey, output, intDomain), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property within domain #3", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output, intDomain, [] (const int v) { return v - 10; }));
            REQUIRE(output == pintNumVal - 10);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintStrKey)], output, intDomain, Numeric::clamp(nintNumVal, pintNumVal - 10)));
            REQUIRE(output == pintNumVal - 10);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output, intDomain, intAdd10));
            REQUIRE(output == pintNumVal + 10);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handle(jsonDoc[kv(pintStrKey)], output, intDomain, Numeric::max(pintNumVal - 10)),
                Basic::DataError
            );
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handle(jsonDoc[kv(nullKey)], output, intDomain, intThrow));
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintNumKey, output, intDomain, [] (const int v) { return v - 10; }));
            REQUIRE(output == pintNumVal - 10);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintStrKey, output, intDomain, Numeric::clamp(nintNumVal, pintNumVal - 10)));
            REQUIRE(output == pintNumVal - 10);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handleKey(jsonDoc, pintNumKey, output, intDomain, intThrow),
                Basic::DataError
            );
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handleKey(jsonDoc, pintStrKey, output, intDomain, Numeric::max(pintNumVal - 10)),
                Basic::DataError
            );
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nepKey, output, intDomain, intThrow));
            REQUIRE(output == intCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property within domain #4", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(trueKey)], output, strDomain));
            REQUIRE(output == Text::yesNo<Meta::Mbs>(trueVal));
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(trueStr2Key)], output, strDomain));
            REQUIRE(output == trueStr2Val);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(fiveNumKey)], output, strDomain));
            REQUIRE(output == fiveNumStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(sevenStrKey)], output, strDomain));
            REQUIRE(output == sevenStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output, strDomain));
            REQUIRE(output == pintStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintStrKey)], output, strDomain));
            REQUIRE(output == pintStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handle(jsonDoc[kv(nullKey)], output, strDomain));
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(trueStr1Key)], output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(falseStr2Key)], output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(zeroNumKey)], output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(zeroStrKey)], output, strDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property within domain #5", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, falseKey, output, svwDomain));
            REQUIRE(output == Text::yesNo<Meta::Mbs>(falseVal));
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, falseStr2Key, output, svwDomain));
            REQUIRE(output == falseStr2Val);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, fiveNumKey, output, svwDomain));
            REQUIRE(output == fiveNumStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, sevenStrKey, output, svwDomain));
            REQUIRE(output == sevenStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintNumKey, output, svwDomain));
            REQUIRE(output == pintStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintStrKey, output, svwDomain));
            REQUIRE(output == pintStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nullKey, output, svwDomain));
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, falseStr1Key, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, trueStr2Key, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, zeroNumKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, zeroStrKey, output, svwDomain), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property within domain #6", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::string output { strCheckVal };
            REQUIRE(
                Json::handle(
                    jsonDoc[kv(trueStr2Key)], output, strDomain,
                    [] (const std::string & v) { return "~"s + v + "~"s; }
                )
            );
            REQUIRE(output == "~"s + trueStr2Val + "~"s);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output, strDomain, strAddTilda));
            REQUIRE(output == "~"s + pintStrVal + "~"s);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handle(
                    jsonDoc[kv(pintStrKey)], output, strDomain,
                    Text::Mbs::maxLength(pintStrVal.size() - 1)
                ),
                Basic::DataError
            );
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(strKey)], output, strDomain, Text::Mbs::lower()));
            REQUIRE_FALSE(output == strVal);
            REQUIRE(output == loweredStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handle(jsonDoc[kv(nullKey)], output, strDomain, strThrow));
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(
                Json::handleKey(
                    jsonDoc, trueStr2Key, output, strDomain,
                    [] (const std::string & v) { return "~"s + v + "~"s; }
                )
            );
            REQUIRE(output == "~"s + trueStr2Val + "~"s);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintNumKey, output, strDomain, strAddTilda));
            REQUIRE(output == "~"s + pintStrVal + "~"s);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handleKey(
                    jsonDoc, pintStrKey, output, strDomain,
                    Text::Mbs::maxLength(pintStrVal.size() - 1)
                ),
                Basic::DataError
            );
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, strKey, output, strDomain, strAddTilda));
            REQUIRE_FALSE(output == strVal);
            REQUIRE_FALSE(output == loweredStrVal);
            REQUIRE_FALSE(output == "~"s + strVal + "~"s);
            REQUIRE(output == "~"s + loweredStrVal + "~"s);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nullKey, output, strDomain, strThrow));
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nepKey, output, strDomain, Text::Mbs::lower()));
            REQUIRE(output == strCheckVal);
        }
    }
}
