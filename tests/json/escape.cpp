// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <json/escape.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// TODO: Дополнить тексты

namespace UnitTests {
    TEST_CASE("Escaping JSON string value", "[json]") {
        {
            constexpr std::wstring_view unicodeString {
                L"* Test string *"
                L"* Тестовая строка *"
                L"* 测试线 *"
                L"* 테스트 라인 *"
                L"* \"'\\/\b\f\n\r\t\v *"
                L"* !@#$%^&*()_+-={}[]|;<>?~ *"
                L"* \xD83D\xDE00 *"
            };

            constexpr std::wstring_view basicEscapedString {
                L"* Test string *"
                L"* Тестовая строка *"
                L"* 测试线 *"
                L"* 테스트 라인 *"
                L"* \\\"'\\\\\\/\\b\\f\\n\\r\\t\\u000B *"
                L"* !@#$%^&*()_+-={}[]|;<>?~ *"
                L"* \xD83D\xDE00 *"
            };

            constexpr std::wstring_view fullEscapedString {
                L"* Test string *"
                L"* \\u0422\\u0435\\u0441\\u0442\\u043E\\u0432\\u0430\\u044F \\u0441\\u0442\\u0440\\u043E\\u043A\\u0430 *"
                L"* \\u6D4B\\u8BD5\\u7EBF *"
                L"* \\uD14C\\uC2A4\\uD2B8 \\uB77C\\uC778 *"
                L"* \\\"'\\\\\\/\\b\\f\\n\\r\\t\\u000B *"
                L"* !@#$%^&*()_+-={}[]|;<>?~ *"
                L"* \\u{01F600} *"
            };

            REQUIRE(Json::escapeBasic(unicodeString) == basicEscapedString);
            REQUIRE(Json::escapeFull(unicodeString) == fullEscapedString);
        }

        {
            constexpr std::string_view unicodeString {
                "* Test string *"
                "* Тестовая строка *"
                "* 测试线 *"
                "* 테스트 라인 *"
                "* \"'\\/\b\f\n\r\t\v *"
                "* !@#$%^&*()_+-={}[]|;<>?~ *"
            };

            constexpr std::string_view basicEscapedString {
                "* Test string *"
                "* Тестовая строка *"
                "* 测试线 *"
                "* 테스트 라인 *"
                "* \\\"'\\\\\\/\\b\\f\\n\\r\\t\\u000B *"
                "* !@#$%^&*()_+-={}[]|;<>?~ *"
            };

            REQUIRE(Json::escapeBasic(unicodeString) == basicEscapedString);
        }
    }
}
