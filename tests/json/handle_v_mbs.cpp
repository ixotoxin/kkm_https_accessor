// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/xetters.h>
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Handle MBS JSON value or object property #1", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            bool output { false };
            REQUIRE(Json::handle(jsonDoc[kv(trueKey)], output));
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE(Json::handle(jsonDoc[kv(trueStr1Key)], output));
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE(Json::handle(jsonDoc[kv(trueStr2Key)], output));
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE(Json::handle(jsonDoc[kv(trueStr3Key)], output));
            REQUIRE(output);
        }

        {
            bool output { true };
            REQUIRE(Json::handle(jsonDoc[kv(falseKey)], output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE(Json::handle(jsonDoc[kv(falseStr1Key)], output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE(Json::handle(jsonDoc[kv(falseStr2Key)], output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE(Json::handle(jsonDoc[kv(falseStr3Key)], output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_FALSE(Json::handle(jsonDoc[kv(nullKey)], output));
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_FALSE(Json::handle(jsonDoc[kv(nullKey)], output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(strKey)], output), Basic::DataError);
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(strKey)], output), Basic::DataError);
            REQUIRE_FALSE(output);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property #2", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            bool output { false };
            REQUIRE(Json::handleKey(jsonDoc, trueKey, output));
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE(Json::handleKey(jsonDoc, trueStr1Key, output));
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE(Json::handleKey(jsonDoc, trueStr2Key, output));
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE(Json::handleKey(jsonDoc, trueStr3Key, output));
            REQUIRE(output);
        }

        {
            bool output { true };
            REQUIRE(Json::handleKey(jsonDoc, falseKey, output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE(Json::handleKey(jsonDoc, falseStr1Key, output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE(Json::handleKey(jsonDoc, falseStr2Key, output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE(Json::handleKey(jsonDoc, falseStr3Key, output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nullKey, output));
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nullKey, output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nepKey, output));
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nepKey, output));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, strKey, output), Basic::DataError);
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, strKey, output), Basic::DataError);
            REQUIRE_FALSE(output);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property #3", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            bool output { true };
            REQUIRE(Json::handle(jsonDoc[kv(trueKey)], output, [] (const bool val) { return !val; }));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handle(
                    jsonDoc[kv(falseKey)], output,
                    [] (const bool) -> bool { throw Basic::DataError { Basic::Wcs::c_invalidValue }; }
                ),
                Basic::DataError
            );
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handleKey(
                    jsonDoc, trueKey, output,
                    [] (const bool) -> bool { throw Basic::DataError { Basic::Wcs::c_invalidValue }; }
                ),
                Basic::DataError
            );
            REQUIRE_FALSE(output);
        }

        {
            bool output { false };
            REQUIRE(Json::handleKey(jsonDoc, falseKey, output, [] (const bool val) { return !val; }));
            REQUIRE(output);
        }

        auto filter1 = [] (const bool val) { return !val; };
        auto filter2 = [] (bool) -> bool { throw Basic::DataError { Basic::Wcs::c_invalidValue }; };

        {
            bool output { true };
            REQUIRE(Json::handle(jsonDoc[kv(trueKey)], output, filter1));
            REQUIRE_FALSE(output);
        }

        {
            bool output { true };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(falseKey)], output, filter2), Basic::DataError);
            REQUIRE(output);
        }

        {
            bool output { false };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, trueKey, output, filter2), Basic::DataError);
            REQUIRE_FALSE(output);
        }

        {
            bool output { false };
            REQUIRE(Json::handleKey(jsonDoc, falseKey, output, filter1));
            REQUIRE(output);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property #4", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintStrKey)], output));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(nintNumKey)], output));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(nintStrKey)], output));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handle(jsonDoc[kv(nullKey)], output));
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(strKey)], output), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property #5", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintNumKey, output));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintStrKey, output));
            REQUIRE(output == pintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, nintNumKey, output));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, nintStrKey, output));
            REQUIRE(output == nintNumVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nullKey, output));
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, strKey, output), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nepKey, output));
            REQUIRE(output == intCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property #6", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output, [] (const int val) { return val - 10; }));
            REQUIRE(output == pintNumVal - 10);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handle(jsonDoc[kv(pintStrKey)], output, Numeric::max(pintNumVal - 10)),
                Basic::DataError
            );
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handleKey(jsonDoc, pintNumKey, output, Numeric::max(pintNumVal - 10)),
                Basic::DataError
            );
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintStrKey, output, [] (const int val) { return val - 10; }));
            REQUIRE(output == pintNumVal - 10);
        }

        auto filter1 = [] (const int val) { return val + 10; };
        auto filter2 = [] (int) -> int { throw Basic::DataError { Basic::Wcs::c_invalidValue }; };

        {
            int output { intCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(nintNumKey)], output, filter1));
            REQUIRE(output == nintNumVal + 10);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(nintStrKey)], output, filter2), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, nintNumKey, output, filter2), Basic::DataError);
            REQUIRE(output == intCheckVal);
        }

        {
            int output { intCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, nintStrKey, output, filter1));
            REQUIRE(output == nintNumVal + 10);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property #7", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintNumKey)], output));
            REQUIRE(output == pintStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(pintStrKey)], output));
            REQUIRE(output == pintStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(strKey)], output));
            REQUIRE(output == strVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handle(jsonDoc[kv(nullKey)], output));
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handle(jsonDoc[kv(emptyObjectKey)], output), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property #8", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintNumKey, output));
            REQUIRE(output == pintStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, pintStrKey, output));
            REQUIRE(output == pintStrVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, strKey, output));
            REQUIRE(output == strVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nullKey, output));
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(std::ignore = Json::handleKey(jsonDoc, emptyObjectKey, output), Basic::DataError);
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_FALSE(Json::handleKey(jsonDoc, nepKey, output));
            REQUIRE(output == strCheckVal);
        }
    }

    TEST_CASE("Handle MBS JSON value or object property #9", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(strKey)], output, Text::Mbs::maxLength(strVal.size() + 1)));
            REQUIRE(output == strVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handle(jsonDoc[kv(strKey)], output, Text::Mbs::maxLength(strVal.size() - 1)),
                Basic::DataError
            );
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handle(jsonDoc[kv(strKey)], output, Text::Mbs::lower()));
            REQUIRE_FALSE(output == strVal);
            REQUIRE(output == Text::lowered(strVal));
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, strKey, output, Text::Mbs::maxLength(strVal.size() + 1)));
            REQUIRE(output == strVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE_THROWS_AS(
                std::ignore = Json::handleKey(jsonDoc, strKey, output, Text::Mbs::maxLength(strVal.size() - 1)),
                Basic::DataError
            );
            REQUIRE(output == strCheckVal);
        }

        {
            std::string output { strCheckVal };
            REQUIRE(Json::handleKey(jsonDoc, strKey, output, Text::Mbs::lower()));
            REQUIRE_FALSE(output == strVal);
            REQUIRE(output == Text::lowered(strVal));
        }
    }
}
