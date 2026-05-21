// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/xetters.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Setting MbS JSON object property via native 'value'", "[json][mbs]") {
        using Catch::Matchers::WithinRel;

        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        Json::Mbs::Value array { Json::Type::Array };
        for (int i { 8 }; i; --i) {
            array.PushBack(Json::Mbs::Value(i), allocator);
        }

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, Json::Mbs::Value { 12345 }));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, Json::Mbs::Value { 12345.67 }, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, Json::Mbs::Value { "Cookie", allocator }));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, std::move(array), allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, Json::Mbs::Value(Json::Type::Null)));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, Json::Mbs::Value(Json::Type::Object), allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, Json::Mbs::Value { true }, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, Json::Mbs::Value { false }, allocator));

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop1"_key)].GetInt64() == 12345);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv("prop2"_key)].GetDouble(), WithinRel(12345.67, 0.000001));

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop3"_key)].GetString() == "Cookie"s);

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsArray());

        auto & jsonArray = jsonDoc[kv("prop4"_key)];
        for (SizeType i { 0 }, v { 8 }; i < jsonArray.Size(); ++i, --v) {
            REQUIRE(jsonArray[i].IsInt64());
            REQUIRE(jsonArray[i].GetInt64() == v);
        }

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsNull());

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsObject());
        REQUIRE(jsonDoc[kv(etpKey)].ObjectEmpty());

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsTrue());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].GetBool());

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsFalse());
        REQUIRE_FALSE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetBool());
    }
}
