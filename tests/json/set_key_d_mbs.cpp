// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/xetters.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Setting MBS JSON object property to Value of Domain #1", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, 1, intDomain));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, 3, intDomain, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, pintNumVal, intDomain));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, pintNumVal, intDomain, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc, "prop5"_key, 321, intDomain), Basic::DataError);
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, 5, intDomain));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, nintNumVal, intDomain, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, 7, intDomain, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, nintNumVal, intDomain, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc[kv(subKey)], "prop3"_key, 654, intDomain, allocator),Basic::DataError);

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop1"_key)].GetInt64() == 1);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop2"_key)].GetInt64() == 3);

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop3"_key)].GetInt64() == pintNumVal);

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsInt64());
        REQUIRE(jsonDoc[kv("prop4"_key)].GetInt64() == pintNumVal);

        REQUIRE_FALSE(jsonDoc.HasMember("prop5"_key));

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsInt64());
        REQUIRE(jsonDoc[kv(enpKey)].GetInt64() == 5);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsInt64());
        REQUIRE(jsonDoc[kv(etpKey)].GetInt64() == nintNumVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsInt64());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].GetInt64() == 7);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsInt64());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetInt64() == nintNumVal);

        REQUIRE_FALSE(jsonDoc[kv(subKey)].HasMember("prop3"_key));
    }

    TEST_CASE("Setting MBS JSON object property to Value of Domain #2", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, "1", strDomain));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, "3", strDomain, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, pintStrVal, strDomain));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, pintStrVal, strDomain, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc, "prop5"_key, "321"s, strDomain), Basic::DataError);
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc, cookieKey, coalVal, strDomain, allocator), Basic::DataError);
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, "5"s, strDomain));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, strVal, strDomain, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, "7"s, strDomain, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, nintStrVal, strDomain, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc[kv(subKey)], "prop3"_key, "654"s, strDomain, allocator), Basic::DataError);
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc[kv(subKey)], cookieKey, coalVal, strDomain, allocator), Basic::DataError);

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop1"_key)].GetString() == "1"s);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop2"_key)].GetString() == "3"s);

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop3"_key)].GetString() == pintStrVal);

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop4"_key)].GetString() == pintStrVal);

        REQUIRE_FALSE(jsonDoc.HasMember("prop5"_key));

        REQUIRE(jsonDoc.HasMember(cookieKey));
        REQUIRE(jsonDoc[kv(cookieKey)].IsString());
        REQUIRE(jsonDoc[kv(cookieKey)].GetString() == cookieVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsString());
        REQUIRE(jsonDoc[kv(enpKey)].GetString() == "5"s);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsString());
        REQUIRE_FALSE(jsonDoc[kv(etpKey)].GetString() == strVal);
        REQUIRE(jsonDoc[kv(etpKey)].GetString() == loweredStrVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].GetString() == "7"s);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetString() == nintStrVal);

        REQUIRE_FALSE(jsonDoc[kv(subKey)].HasMember("prop3"_key));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(cookieKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(cookieKey)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(cookieKey)].GetString() == cookieVal);
    }

    TEST_CASE("Setting MBS JSON object property to Value of Domain #3", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { Json::Type::Object, &allocator };
        prepareObject(jsonDoc);

        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop1"_key, "1", svwDomain));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop2"_key, "3", svwDomain, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop3"_key, pintStrVal, svwDomain));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, "prop4"_key, pintStrVal, svwDomain, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc, "prop5"_key, "321", svwDomain), Basic::DataError);
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc, cookieKey, coalVal, svwDomain, allocator), Basic::DataError);
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, enpKey, "5"s, svwDomain));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc, etpKey, strVal, svwDomain, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop1"_key, "7", svwDomain, allocator));
        REQUIRE_NOTHROW(Json::setKey(jsonDoc[kv(subKey)], "prop2"_key, nintStrVal, svwDomain, allocator));
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc[kv(subKey)], "prop3"_key, "654"s, svwDomain, allocator), Basic::DataError);
        REQUIRE_THROWS_AS(Json::setKey(jsonDoc[kv(subKey)], cookieKey, coalVal, svwDomain, allocator), Basic::DataError);

        REQUIRE(jsonDoc.HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv("prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop1"_key)].GetString() == "1"s);

        REQUIRE(jsonDoc.HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop2"_key)].GetString() == "3"s);

        REQUIRE(jsonDoc.HasMember("prop3"_key));
        REQUIRE(jsonDoc[kv("prop3"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop3"_key)].GetString() == pintStrVal);

        REQUIRE(jsonDoc.HasMember("prop4"_key));
        REQUIRE(jsonDoc[kv("prop4"_key)].IsString());
        REQUIRE(jsonDoc[kv("prop4"_key)].GetString() == pintStrVal);

        REQUIRE_FALSE(jsonDoc.HasMember("prop5"_key));

        REQUIRE(jsonDoc.HasMember(cookieKey));
        REQUIRE(jsonDoc[kv(cookieKey)].IsString());
        REQUIRE(jsonDoc[kv(cookieKey)].GetString() == cookieVal);

        REQUIRE(jsonDoc.HasMember(enpKey));
        REQUIRE(jsonDoc[kv(enpKey)].IsString());
        REQUIRE(jsonDoc[kv(enpKey)].GetString() == "5"s);

        REQUIRE(jsonDoc.HasMember(etpKey));
        REQUIRE(jsonDoc[kv(etpKey)].IsString());
        REQUIRE_FALSE(jsonDoc[kv(etpKey)].GetString() == strVal);
        REQUIRE(jsonDoc[kv(etpKey)].GetString() == loweredStrVal);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop1"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop1"_key)].GetString() == "7"s);

        REQUIRE(jsonDoc[kv(subKey)].HasMember("prop2"_key));
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv("prop2"_key)].GetString() == nintStrVal);

        REQUIRE_FALSE(jsonDoc[kv(subKey)].HasMember("prop3"_key));

        REQUIRE(jsonDoc[kv(subKey)].HasMember(cookieKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(cookieKey)].IsString());
        REQUIRE(jsonDoc[kv(subKey)][kv(cookieKey)].GetString() == cookieVal);
    }
}
