// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/xetters.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Setting MBS JSON object property to String Value (std::tm)", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, tmVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, tmVal, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, tmVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, tmVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop"_key, tmVal, allocator));

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop1"_key)].GetString() == tmStrVal);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop2"_key)].GetString() == tmStrVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsString());
        REQUIRE(jsonDoc[kv(enpKey)].GetString() == tmStrVal);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsString());
        REQUIRE(jsonDoc[kv(etpKey)].GetString() == tmStrVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop"_key)].GetString() == tmStrVal);
    }
}
