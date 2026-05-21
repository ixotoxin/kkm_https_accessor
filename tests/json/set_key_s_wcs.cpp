// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/xetters.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Setting WCS JSON object property to String Value (from std::basic_string)", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop1"_key, L"Cookie"));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop2"_key, L"Cookie", allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop3"_key, strVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop4"_key, strVal, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, L"Cookie"s));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, strVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], L"prop1"_key, L"Cookie"s, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], L"prop2"_key, strVal, allocator));

        REQUIRE(jsonDoc.HasMember(L"prop1"_key));
        REQUIRE(jsonDoc[kv(L"prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv(L"prop1"_key)].GetString() == L"Cookie"s);

        REQUIRE(jsonDoc.HasMember(L"prop2"_key));
        REQUIRE(jsonDoc[kv(L"prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv(L"prop2"_key)].GetString() == L"Cookie"s);

        REQUIRE(jsonDoc.HasMember(L"prop3"_key));
        REQUIRE(jsonDoc[kv(L"prop3"_key)].IsString());
        REQUIRE(jsonDoc[kv(L"prop3"_key)].GetString() == strVal);

        REQUIRE(jsonDoc.HasMember(L"prop4"_key));
        REQUIRE(jsonDoc[kv(L"prop4"_key)].IsString());
        REQUIRE(jsonDoc[kv(L"prop4"_key)].GetString() == strVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsString());
        REQUIRE(jsonDoc[kv(enpKey)].GetString() == L"Cookie"s);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsString());
        REQUIRE(jsonDoc[kv(etpKey)].GetString() == strVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember(L"prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].GetString() == L"Cookie"s);

        REQUIRE(jsonDoc[kv(subKey)].HasMember(L"prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].GetString() == strVal);
    }

    TEST_CASE("Setting WCS JSON object property to String Value (from std::basic_string_view)", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop1"_key, L"Cookie"sv));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop2"_key, L"Cookie"sv, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop3"_key, cookieVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop4"_key, cookieVal, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, L"Cookie"sv));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, cookieVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], L"prop1"_key, L"Cookie"sv, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], L"prop2"_key, cookieVal, allocator));

        REQUIRE(jsonDoc.HasMember(L"prop1"_key));
        REQUIRE(jsonDoc[kv(L"prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv(L"prop1"_key)].GetString() == L"Cookie"s);

        REQUIRE(jsonDoc.HasMember(L"prop2"_key));
        REQUIRE(jsonDoc[kv(L"prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv(L"prop2"_key)].GetString() == L"Cookie"s);

        REQUIRE(jsonDoc.HasMember(L"prop3"_key));
        REQUIRE(jsonDoc[kv(L"prop3"_key)].IsString());
        REQUIRE(jsonDoc[kv(L"prop3"_key)].GetString() == cookieVal);

        REQUIRE(jsonDoc.HasMember(L"prop4"_key));
        REQUIRE(jsonDoc[kv(L"prop4"_key)].IsString());
        REQUIRE(jsonDoc[kv(L"prop4"_key)].GetString() == cookieVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsString());
        REQUIRE(jsonDoc[kv(enpKey)].GetString() == L"Cookie"s);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsString());
        REQUIRE(jsonDoc[kv(etpKey)].GetString() == cookieVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember(L"prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].GetString() == L"Cookie"s);

        REQUIRE(jsonDoc[kv(subKey)].HasMember(L"prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].GetString() == cookieVal);
    }
}
