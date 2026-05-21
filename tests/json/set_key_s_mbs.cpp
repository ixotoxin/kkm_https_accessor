// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/xetters.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Setting MBS JSON object property to String Value (from std::basic_string)", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, "Cookie"));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, "Cookie", allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, strVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, strVal, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, "Cookie"s));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, strVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, "Cookie"s, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, strVal, allocator));

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop1"_key)].GetString() == "Cookie"s);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop2"_key)].GetString() == "Cookie"s);

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop3"_key)].GetString() == strVal);

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop4"_key)].GetString() == strVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsString());
        REQUIRE(jsonDoc[kv(enpKey)].GetString() == "Cookie"s);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsString());
        REQUIRE(jsonDoc[kv(etpKey)].GetString() == strVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].GetString() == "Cookie"s);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetString() == strVal);
    }

    TEST_CASE("Setting MBS JSON object property to String Value (from std::basic_string_view)", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, "Cookie"sv));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, "Cookie"sv, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, cookieVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, cookieVal, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, "Cookie"sv));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, cookieVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, "Cookie"sv, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, cookieVal, allocator));

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop1"_key)].GetString() == "Cookie"s);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop2"_key)].GetString() == "Cookie"s);

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop3"_key)].GetString() == cookieVal);

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop4"_key)].GetString() == cookieVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsString());
        REQUIRE(jsonDoc[kv(enpKey)].GetString() == "Cookie"s);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsString());
        REQUIRE(jsonDoc[kv(etpKey)].GetString() == cookieVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].GetString() == "Cookie"s);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetString() == cookieVal);
    }
}
