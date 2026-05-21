// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/xetters.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Setting WCS JSON object property via native 'value'", "[json][wcs]") {
        using Catch::Matchers::WithinRel;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        Json::Wcs::Value array { Json::Type::Array };
        for (int i { 8 }; i; --i) {
            array.PushBack(Json::Wcs::Value(i), allocator);
        }

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop1"_key, Json::Wcs::Value { 12345 }));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop2"_key, Json::Wcs::Value { 12345.67 }, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop3"_key, Json::Wcs::Value { L"Cookie", allocator }));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop4"_key, std::move(array), allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, Json::Wcs::Value(Json::Type::Null)));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, Json::Wcs::Value(Json::Type::Object), allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], L"prop1"_key, Json::Wcs::Value { true }, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], L"prop2"_key, Json::Wcs::Value { false }, allocator));

        REQUIRE(jsonDoc.HasMember(L"prop1"_key));
        REQUIRE(jsonDoc[kv(L"prop1"_key)].IsInt64());
        REQUIRE(jsonDoc[kv(L"prop1"_key)].GetInt64() == 12345);

        REQUIRE(jsonDoc.HasMember(L"prop2"_key));
        REQUIRE(jsonDoc[kv(L"prop2"_key)].IsDouble());
        REQUIRE_THAT(jsonDoc[kv(L"prop2"_key)].GetDouble(), WithinRel(12345.67, 0.000001));

        REQUIRE(jsonDoc.HasMember(L"prop3"_key));
        REQUIRE(jsonDoc[kv(L"prop3"_key)].IsString());
        REQUIRE(jsonDoc[kv(L"prop3"_key)].GetString() == L"Cookie"s);

        REQUIRE(jsonDoc.HasMember(L"prop4"_key));
        REQUIRE(jsonDoc[kv(L"prop4"_key)].IsArray());

        auto & jsonArray = jsonDoc[kv(L"prop4"_key)];
        for (SizeType i { 0 }, v { 8 }; i < jsonArray.Size(); ++i, --v) {
            REQUIRE(jsonArray[i].IsInt64());
            REQUIRE(jsonArray[i].GetInt64() == v);
        }

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsNull());

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsObject());
        REQUIRE(jsonDoc[kv(etpKey)].ObjectEmpty());

        REQUIRE(jsonDoc[kv(subKey)].HasMember(L"prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].IsTrue());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].GetBool());

        REQUIRE(jsonDoc[kv(subKey)].HasMember(L"prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].IsFalse());
        REQUIRE_FALSE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].GetBool());
    }
}
