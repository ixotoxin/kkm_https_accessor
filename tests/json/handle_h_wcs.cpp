// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/xetters.h>
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Handle WCS JSON value or object property with callback", "[json][wcs]") {
        using Json::Wcs::Value;
        using Json::PtrView;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        {
            SizeType output {};
            REQUIRE(
                Json::handle(jsonDoc, [& output] (const Value & json, const PtrView) -> bool {
                    REQUIRE(json.IsObject());
                    output = json.MemberCount();
                    return true;
                })
            );
            REQUIRE(output == 26);
        }

        {
            SizeType output {};
            REQUIRE_FALSE(
                Json::handle(jsonDoc, [& output] (const Value & json, const PtrView) -> bool {
                    REQUIRE(json.IsObject());
                    output = json.MemberCount();
                    return false;
                })
            );
            REQUIRE(output == 26);
        }

        REQUIRE_THROWS_AS(
            std::ignore = Json::handle(jsonDoc, [] (const Value & json, const PtrView) -> bool {
                REQUIRE(json.IsObject());
                REQUIRE(json.HasMember(strKey));
                std::ignore = Json::cast<bool>(json[kv(strKey)]);
                return true;
            }),
            Basic::DataError
        );

        {
            SizeType output {};
            REQUIRE(
                Json::handleKey(jsonDoc, subKey, [& output] (const Value & json, const PtrView) -> bool {
                    REQUIRE(json.IsObject());
                    output = json.MemberCount();
                    return true;
                })
            );
            REQUIRE(output == 25);
        }

        {
            SizeType output {};
            REQUIRE_FALSE(
                Json::handleKey(jsonDoc, subKey, [& output] (const Value & json, const PtrView) -> bool {
                    REQUIRE(json.IsObject());
                    output = json.MemberCount();
                    return false;
                })
            );
            REQUIRE(output == 25);
        }

        REQUIRE_THROWS_AS(
            std::ignore = Json::handleKey(jsonDoc, subKey, [] (const Value & json, const PtrView) -> bool {
                REQUIRE(json.IsObject());
                REQUIRE(json.HasMember(strKey));
                std::ignore = Json::cast<bool>(json[kv(strKey)]);
                return true;
            }),
            Basic::DataError
        );

        REQUIRE_FALSE(
            Json::handleKey(jsonDoc, nepKey, [] (const Value &, const PtrView) -> bool {
                return true;
            })
        );
    }
}
