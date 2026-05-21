// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/xetters.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Setting WCS JSON object property to String Value (std::tm)", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop1"_key, tmVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop2"_key, tmVal, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, tmVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, tmVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], L"prop"_key, tmVal, allocator));

        REQUIRE(jsonDoc.HasMember(L"prop1"_key));
        REQUIRE(jsonDoc[kv(L"prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv(L"prop1"_key)].GetString() == tmStrVal);

        REQUIRE(jsonDoc.HasMember(L"prop2"_key));
        REQUIRE(jsonDoc[kv(L"prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv(L"prop2"_key)].GetString() == tmStrVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsString());
        REQUIRE(jsonDoc[kv(enpKey)].GetString() == tmStrVal);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsString());
        REQUIRE(jsonDoc[kv(etpKey)].GetString() == tmStrVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember(L"prop"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop"_key)].GetString() == tmStrVal);
    }
}
