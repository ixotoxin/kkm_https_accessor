// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/xetters.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Setting MBS JSON object property to Integer Value", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, 12345));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, 12345, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, pintNumVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, pintNumVal, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, -12345));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, nintNumVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, -12345, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, nintNumVal, allocator));

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsNumber());
        REQUIRE(jsonDoc[kv("prop1"_key)].IsInt());
        REQUIRE(jsonDoc[kv("prop1"_key)].IsUint());
        REQUIRE(jsonDoc[kv("prop1"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop1"_key)].IsUint64());
        REQUIRE(jsonDoc[kv("prop1"_key)].GetInt() == 12345);
        REQUIRE(jsonDoc[kv("prop1"_key)].GetUint() == 12345);
        REQUIRE(jsonDoc[kv("prop1"_key)].GetInt64() == 12345);
        REQUIRE(jsonDoc[kv("prop1"_key)].GetUint64() == 12345);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop2"_key)].GetInt64() == 12345);

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop3"_key)].GetInt64() == pintNumVal);

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop4"_key)].GetInt64() == pintNumVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsInt64());
        REQUIRE(jsonDoc[kv(enpKey)].GetInt64() == -12345);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsInt64());
        REQUIRE(jsonDoc[kv(etpKey)].GetInt64() == nintNumVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsInt64());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].GetInt64() == -12345);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsNumber());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsInt());
        REQUIRE_FALSE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsUint());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsInt64());
        REQUIRE_FALSE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsUint64());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetInt() == nintNumVal);
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetInt64() == nintNumVal);
    }

    TEST_CASE("Setting MBS JSON object property to Double Value", "[json][mbs]") {
        using Catch::Matchers::WithinRel;

        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, 12345.67));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, 12345.67, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, pdblNumVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, pdblNumVal, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, -12345.67));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, ndblNumVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, -12345.67, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, ndblNumVal, allocator));

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsNumber());
        REQUIRE(jsonDoc[kv("prop1"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv("prop1"_key)].GetDouble(), WithinRel(12345.67, 0.000001));

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv("prop2"_key)].GetDouble(), WithinRel(12345.67, 0.000001));

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv("prop3"_key)].GetDouble(), WithinRel(pdblNumVal, 0.000001));

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv("prop4"_key)].GetDouble(), WithinRel(pdblNumVal, 0.000001));

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(enpKey)].GetDouble(), WithinRel(-12345.67, 0.000001));

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(etpKey)].GetDouble(), WithinRel(ndblNumVal, 0.000001));

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(subKey)][kv("prop1"_key)].GetDouble(), WithinRel(-12345.67, 0.000001));

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(subKey)][kv("prop2"_key)].GetDouble(), WithinRel(ndblNumVal, 0.000001));
    }
}
