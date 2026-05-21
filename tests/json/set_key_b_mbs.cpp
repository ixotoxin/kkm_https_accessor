// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/xetters.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Setting MBS JSON object property to True", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, true));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, true, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, trueVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, trueVal, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, true));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, trueVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, true, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, trueVal, allocator));

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsBool());
        REQUIRE(jsonDoc[kv("prop1"_key)].IsTrue());
        REQUIRE(jsonDoc[kv("prop1"_key)].GetBool() == true);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsBool());
        REQUIRE(jsonDoc[kv("prop2"_key)].IsTrue());
        REQUIRE(jsonDoc[kv("prop2"_key)].GetBool() == true);

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsBool());
        REQUIRE(jsonDoc[kv("prop3"_key)].IsTrue());
        REQUIRE(jsonDoc[kv("prop3"_key)].GetBool() == trueVal);

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsBool());
        REQUIRE(jsonDoc[kv("prop4"_key)].IsTrue());
        REQUIRE(jsonDoc[kv("prop4"_key)].GetBool() == trueVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsBool());
        REQUIRE(jsonDoc[kv(enpKey)].IsTrue());
        REQUIRE(jsonDoc[kv(enpKey)].GetBool() == true);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsBool());
        REQUIRE(jsonDoc[kv(etpKey)].IsTrue());
        REQUIRE(jsonDoc[kv(etpKey)].GetBool() == trueVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsTrue());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].GetBool() == true);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsTrue());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetBool() == trueVal);
    }

    TEST_CASE("Setting MBS JSON object property to False", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, false));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, false, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, falseVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, falseVal, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, false));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, falseVal));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, false, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, falseVal, allocator));

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsBool());
        REQUIRE(jsonDoc[kv("prop1"_key)].IsFalse());
        REQUIRE(jsonDoc[kv("prop1"_key)].GetBool() == false);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsBool());
        REQUIRE(jsonDoc[kv("prop2"_key)].IsFalse());
        REQUIRE(jsonDoc[kv("prop2"_key)].GetBool() == false);

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsBool());
        REQUIRE(jsonDoc[kv("prop3"_key)].IsFalse());
        REQUIRE(jsonDoc[kv("prop3"_key)].GetBool() == falseVal);

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsBool());
        REQUIRE(jsonDoc[kv("prop4"_key)].IsFalse());
        REQUIRE(jsonDoc[kv("prop4"_key)].GetBool() == falseVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsBool());
        REQUIRE(jsonDoc[kv(enpKey)].IsFalse());
        REQUIRE(jsonDoc[kv(enpKey)].GetBool() == false);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsBool());
        REQUIRE(jsonDoc[kv(etpKey)].IsFalse());
        REQUIRE(jsonDoc[kv(etpKey)].GetBool() == falseVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsFalse());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].GetBool() == false);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsBool());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsFalse());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetBool() == falseVal);
    }
}
