// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/xetters.h>
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Handle MBS JSON value or object property as range", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        // TODO: Реализовать
    }
}
