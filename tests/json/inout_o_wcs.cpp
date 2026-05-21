// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Write WCS JSON object to std::wostream", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        std::wstring original { jsonObjStr };

        REQUIRE_NOTHROW(jsonDoc <<= original);

        std::wstringstream output {};

        REQUIRE_NOTHROW(jsonDoc >>= output);

        std::wstring written { output.str() };

        REQUIRE(removeWhiteSpace(std::move(original)) == removeWhiteSpace(std::move(written)));
    }

    TEST_CASE("Write WCS JSON object to std::ostream", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        std::wstring original { jsonObjStr };

        REQUIRE_NOTHROW(jsonDoc <<= original);

        std::stringstream output {};

        REQUIRE_NOTHROW(jsonDoc >>= output);

        std::string written { output.str() };

        REQUIRE(removeWhiteSpace(Text::convert(written)) == removeWhiteSpace(std::move(original)));
    }
}
