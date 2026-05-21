// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/xetters.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Setting WCS JSON object property to True", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop1"_key, true));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop2"_key, true, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop3"_key, trueVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop4"_key, trueVal, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, true));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, trueVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], L"prop1"_key, true, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], L"prop2"_key, trueVal, allocator));

        REQUIRE(jsonDoc.HasMember(L"prop1"_key));
        REQUIRE(jsonDoc[kv(L"prop1"_key)].IsBool());
        REQUIRE(jsonDoc[kv(L"prop1"_key)].IsTrue());
        REQUIRE(jsonDoc[kv(L"prop1"_key)].GetBool() == true);

        REQUIRE(jsonDoc.HasMember(L"prop2"_key));
        REQUIRE(jsonDoc[kv(L"prop2"_key)].IsBool());
        REQUIRE(jsonDoc[kv(L"prop2"_key)].IsTrue());
        REQUIRE(jsonDoc[kv(L"prop2"_key)].GetBool() == true);

        REQUIRE(jsonDoc.HasMember(L"prop3"_key));
        REQUIRE(jsonDoc[kv(L"prop3"_key)].IsBool());
        REQUIRE(jsonDoc[kv(L"prop3"_key)].IsTrue());
        REQUIRE(jsonDoc[kv(L"prop3"_key)].GetBool() == trueVal);

        REQUIRE(jsonDoc.HasMember(L"prop4"_key));
        REQUIRE(jsonDoc[kv(L"prop4"_key)].IsBool());
        REQUIRE(jsonDoc[kv(L"prop4"_key)].IsTrue());
        REQUIRE(jsonDoc[kv(L"prop4"_key)].GetBool() == trueVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsBool());
        REQUIRE(jsonDoc[kv(enpKey)].IsTrue());
        REQUIRE(jsonDoc[kv(enpKey)].GetBool() == true);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsBool());
        REQUIRE(jsonDoc[kv(etpKey)].IsTrue());
        REQUIRE(jsonDoc[kv(etpKey)].GetBool() == trueVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember(L"prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].IsTrue());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].GetBool() == true);

        REQUIRE(jsonDoc[kv(subKey)].HasMember(L"prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].IsTrue());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].GetBool() == trueVal);
    }

    TEST_CASE("Setting WCS JSON object property to False", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop1"_key, false));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop2"_key, false, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop3"_key, falseVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, L"prop4"_key, falseVal, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, false));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, falseVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], L"prop1"_key, false, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], L"prop2"_key, falseVal, allocator));

        REQUIRE(jsonDoc.HasMember(L"prop1"_key));
        REQUIRE(jsonDoc[kv(L"prop1"_key)].IsBool());
        REQUIRE(jsonDoc[kv(L"prop1"_key)].IsFalse());
        REQUIRE(jsonDoc[kv(L"prop1"_key)].GetBool() == false);

        REQUIRE(jsonDoc.HasMember(L"prop2"_key));
        REQUIRE(jsonDoc[kv(L"prop2"_key)].IsBool());
        REQUIRE(jsonDoc[kv(L"prop2"_key)].IsFalse());
        REQUIRE(jsonDoc[kv(L"prop2"_key)].GetBool() == false);

        REQUIRE(jsonDoc.HasMember(L"prop3"_key));
        REQUIRE(jsonDoc[kv(L"prop3"_key)].IsBool());
        REQUIRE(jsonDoc[kv(L"prop3"_key)].IsFalse());
        REQUIRE(jsonDoc[kv(L"prop3"_key)].GetBool() == falseVal);

        REQUIRE(jsonDoc.HasMember(L"prop4"_key));
        REQUIRE(jsonDoc[kv(L"prop4"_key)].IsBool());
        REQUIRE(jsonDoc[kv(L"prop4"_key)].IsFalse());
        REQUIRE(jsonDoc[kv(L"prop4"_key)].GetBool() == falseVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsBool());
        REQUIRE(jsonDoc[kv(enpKey)].IsFalse());
        REQUIRE(jsonDoc[kv(enpKey)].GetBool() == false);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsBool());
        REQUIRE(jsonDoc[kv(etpKey)].IsFalse());
        REQUIRE(jsonDoc[kv(etpKey)].GetBool() == falseVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember(L"prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].IsFalse());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].GetBool() == false);

        REQUIRE(jsonDoc[kv(subKey)].HasMember(L"prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].IsFalse());
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].GetBool() == falseVal);
    }
}
