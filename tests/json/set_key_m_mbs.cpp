// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/xetters.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Setting MBS JSON object property to Value of Cast Map #1", "[json][mbs]") {
        using Catch::Matchers::WithinRel;

        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, 1, i2dCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, 3, i2dCastMap, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, pintNumVal, i2dCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, pintNumVal, i2dCastMap, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc, "prop5"_key, 321, i2dCastMap), Basic::DataError);
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, 5, i2dCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, nintNumVal, i2dCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, 7, i2dCastMap, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, nintNumVal, i2dCastMap, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc[kv(subKey)], "prop3"_key, 654, i2dCastMap, allocator), Basic::DataError);

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv("prop1"_key)].GetDouble(), WithinRel(11.11, 0.000001));

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv("prop2"_key)].GetDouble(), WithinRel(33.33, 0.000001));

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv("prop3"_key)].GetDouble(), WithinRel(pdblNumVal, 0.000001));

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv("prop4"_key)].GetDouble(), WithinRel(pdblNumVal, 0.000001));

        REQUIRE_FALSE(jsonDoc.HasMember("prop5"_key));

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(enpKey)].GetDouble(), WithinRel(55.55, 0.000001));

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(etpKey)].GetDouble(), WithinRel(ndblNumVal, 0.000001));

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(subKey)][kv("prop1"_key)].GetDouble(), WithinRel(77.77, 0.000001));

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(subKey)][kv("prop2"_key)].GetDouble(), WithinRel(ndblNumVal, 0.000001));

        REQUIRE_FALSE(jsonDoc[kv(subKey)].HasMember("prop3"_key));
    }

    TEST_CASE("Setting MBS JSON object property to Value of Cast Map #2", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, 1, i2sCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, 3, i2sCastMap, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, pintNumVal, i2sCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, pintNumVal, i2sCastMap, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc, "prop5"_key, 321, i2sCastMap), Basic::DataError);
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, 5, i2sCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, nintNumVal, i2sCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, 7, i2sCastMap, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, nintNumVal, i2sCastMap, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc[kv(subKey)], "prop3"_key, 654, i2sCastMap, allocator), Basic::DataError);

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop1"_key)].GetString() == oneStrVal);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop2"_key)].GetString() == threeStrVal);

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop3"_key)].GetString() == cookieVal);

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop4"_key)].GetString() == cookieVal);

        REQUIRE_FALSE(jsonDoc.HasMember("prop5"_key));

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsString());
        REQUIRE(jsonDoc[kv(enpKey)].GetString() == fiveStrVal);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsString());
        REQUIRE(jsonDoc[kv(etpKey)].GetString() == coalVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].GetString() == sevenStrVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetString() == coalVal);

        REQUIRE_FALSE(jsonDoc[kv(subKey)].HasMember("prop3"_key));
    }

    TEST_CASE("Setting MBS JSON object property to Value of Cast Map #3", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, "\t One \t", v2iCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, "\t Three \t"s, v2iCastMap, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, trueStr2Val, v2iCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, falseStr2Val, v2iCastMap, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc, "prop5"_key, "zero"sv, v2iCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc, cookieKey, coalVal, v2iCastMap), Basic::DataError);
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, "\t Five \t", v2iCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, pintStrVal, v2iCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, "\t Seven \t"s, v2iCastMap, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, nintStrVal, v2iCastMap, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc[kv(subKey)], "prop3"_key, "zero"sv, v2iCastMap, allocator), Basic::DataError);
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc[kv(subKey)], cookieKey, coalVal, v2iCastMap, allocator), Basic::DataError);

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop1"_key)].GetInt64() == oneNumVal);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop2"_key)].GetInt64() == threeNumVal);

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop3"_key)].GetInt64() == oneNumVal);

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop4"_key)].GetInt64() == zeroNumVal);

        REQUIRE_FALSE(jsonDoc.HasMember("prop5"_key));

        REQUIRE(jsonDoc.HasMember(cookieKey));
        REQUIRE(jsonDoc[kv(cookieKey)].IsString());
        REQUIRE(jsonDoc[kv(cookieKey)].GetString() == cookieVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsInt64());
        REQUIRE(jsonDoc[kv(enpKey)].GetInt64() == fiveNumVal);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsInt64());
        REQUIRE(jsonDoc[kv(etpKey)].GetInt64() == nintNumVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsInt64());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].GetInt64() == sevenNumVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsInt64());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetInt64() == pintNumVal);

        REQUIRE_FALSE(jsonDoc[kv(subKey)].HasMember("prop3"_key));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(cookieKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(cookieKey)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(cookieKey)].GetString() == cookieVal);
    }

    TEST_CASE("Setting MBS JSON object property to Value of Cast Map #4", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop"_key, "\t One \t", s2vCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, "\t Three \t"s, s2vCastMap, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, trueStr2Val, s2vCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, falseStr2Val, s2vCastMap, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc, "prop5"_key, "zero"sv, s2vCastMap), Basic::DataError);
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc, cookieKey, coalVal, s2vCastMap), Basic::DataError);
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, "\t Five \t", s2vCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, pintStrVal, s2vCastMap));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop"_key, "\t Seven \t"s, s2vCastMap, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, nintStrVal, s2vCastMap, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc[kv(subKey)], "prop3"_key, "zero"sv, s2vCastMap, allocator), Basic::DataError);
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc[kv(subKey)], cookieKey, coalVal, s2vCastMap, allocator), Basic::DataError);

        REQUIRE(jsonDoc.HasMember("prop"_key));
        REQUIRE(jsonDoc[kv("prop"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop"_key)].GetString() == oneNumStrVal);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop2"_key)].GetString() == threeNumStrVal);

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop3"_key)].GetString() == trueStr1Val);

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop4"_key)].GetString() == falseStr1Val);

        REQUIRE_FALSE(jsonDoc.HasMember("prop5"_key));

        REQUIRE(jsonDoc.HasMember(cookieKey));
        REQUIRE(jsonDoc[kv(cookieKey)].IsString());
        REQUIRE(jsonDoc[kv(cookieKey)].GetString() == cookieVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsString());
        REQUIRE(jsonDoc[kv(enpKey)].GetString() == fiveNumStrVal);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsString());
        REQUIRE(jsonDoc[kv(etpKey)].GetString() == cookieVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop"_key)].GetString() == sevenNumStrVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetString() == coalVal);

        REQUIRE_FALSE(jsonDoc[kv(subKey)].HasMember("prop3"_key));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(cookieKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(cookieKey)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(cookieKey)].GetString() == cookieVal);
    }
}
