// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <catch2/catch_test_macros.hpp>
#include <lib/hexer.h>
#include <bit>

namespace UnitTests {
    using namespace std::string_literals;
    using namespace std::string_view_literals;

    TEST_CASE("hexer", "[hex_pair]" ) {
        REQUIRE(0xfe == std::bit_cast<uint8_t>(Bin::Hexer<42>::HexPair { 0xe, 0xf }));
    }

    TEST_CASE("hexer", "[bin_to_hex]" ) {
        {
            uint8_t bin { 0xfe };
            auto hex = std::bit_cast<Bin::Hexer<sizeof(bin)>>(bin);

            {
                size_t pos { 1 };
                std::wstring str(4, L' ');
                hex.writeTo<>(str, pos);
                REQUIRE(str == L" FE "sv);
                hex.writeTo<>(str);
                REQUIRE(str == L" FE FE"sv);
            }

            {
                size_t pos { 1 };
                std::string str(4, ' ');
                hex.writeTo<>(str, pos);
                REQUIRE(str == " FE "sv);
                hex.writeTo<>(str);
                REQUIRE(str == " FE FE"sv);
            }
        }

        {
            uint16_t bin { 0xdcba };
            auto hex = std::bit_cast<Bin::Hexer<sizeof(bin)>>(bin);

            {
                size_t pos { 1 };
                std::wstring str(6, L' ');
                hex.writeTo<>(str, pos);
                REQUIRE(str == L" DCBA "sv);
                hex.writeTo<>(str);
                REQUIRE(str == L" DCBA DCBA"sv);
            }

            {
                size_t pos { 1 };
                std::string str(6, ' ');
                hex.writeTo<>(str, pos);
                REQUIRE(str == " DCBA "sv);
                hex.writeTo<>(str);
                REQUIRE(str == " DCBA DCBA"sv);
            }
        }

        {
            uint32_t bin { 0x9876'5432 };
            auto hex = std::bit_cast<Bin::Hexer<sizeof(bin)>>(bin);

            {
                size_t pos { 1 };
                std::wstring str(10, L' ');
                hex.writeTo<>(str, pos);
                REQUIRE(str == L" 98765432 "sv);
                hex.writeTo<>(str);
                REQUIRE(str == L" 98765432 98765432"sv);
            }

            {
                size_t pos { 1 };
                std::string str(10, ' ');
                hex.writeTo<>(str, pos);
                REQUIRE(str == " 98765432 "sv);
                hex.writeTo<>(str);
                REQUIRE(str == " 98765432 98765432"sv);
            }
        }

        {
            uint64_t bin { 0xfedc'ba98'7654'3210 };
            auto hex = std::bit_cast<Bin::Hexer<sizeof(bin)>>(bin);

            {
                size_t pos { 1 };
                std::wstring str(18, L' ');
                hex.writeTo<>(str, pos);
                REQUIRE(str == L" FEDCBA9876543210 "sv);
                hex.writeTo<>(str);
                REQUIRE(str == L" FEDCBA9876543210 FEDCBA9876543210"sv);
                pos = 1;
                hex.writeTo<2>(str, pos);
                REQUIRE(str == L" 3210BA9876543210 FEDCBA9876543210"sv);
                hex.writeTo<2>(str);
                REQUIRE(str == L" 3210BA9876543210 FEDCBA98765432103210"sv);
                pos = 1;
                hex.writeTo<3, 3>(str, pos);
                REQUIRE(str == L" BA98769876543210 FEDCBA98765432103210"sv);
                hex.writeTo<3, 3>(str);
                REQUIRE(str == L" BA98769876543210 FEDCBA98765432103210BA9876"sv);
                pos = 1;
                hex.writeTo<4>(str, pos);
                REQUIRE(str == L" 7654321076543210 FEDCBA98765432103210BA9876"sv);
                hex.writeTo<4>(str);
                REQUIRE(str == L" 7654321076543210 FEDCBA98765432103210BA987676543210"sv);
                pos = 1;
                hex.writeTo<4, 4>(str, pos);
                REQUIRE(str == L" FEDCBA9876543210 FEDCBA98765432103210BA987676543210"sv);
                hex.writeTo<4, 4>(str);
                REQUIRE(str == L" FEDCBA9876543210 FEDCBA98765432103210BA987676543210FEDCBA98"sv);
            }

            {
                size_t pos { 1 };
                std::string str(18, ' ');
                hex.writeTo<>(str, pos);
                REQUIRE(str == " FEDCBA9876543210 "sv);
                hex.writeTo<>(str);
                REQUIRE(str == " FEDCBA9876543210 FEDCBA9876543210"sv);
                pos = 1;
                hex.writeTo<2>(str, pos);
                REQUIRE(str == " 3210BA9876543210 FEDCBA9876543210"sv);
                hex.writeTo<2>(str);
                REQUIRE(str == " 3210BA9876543210 FEDCBA98765432103210"sv);
                pos = 1;
                hex.writeTo<3, 3>(str, pos);
                REQUIRE(str == " BA98769876543210 FEDCBA98765432103210"sv);
                hex.writeTo<3, 3>(str);
                REQUIRE(str == " BA98769876543210 FEDCBA98765432103210BA9876"sv);
                pos = 1;
                hex.writeTo<4>(str, pos);
                REQUIRE(str == " 7654321076543210 FEDCBA98765432103210BA9876"sv);
                hex.writeTo<4>(str);
                REQUIRE(str == " 7654321076543210 FEDCBA98765432103210BA987676543210"sv);
                pos = 1;
                hex.writeTo<4, 4>(str, pos);
                REQUIRE(str == " FEDCBA9876543210 FEDCBA98765432103210BA987676543210"sv);
                hex.writeTo<4, 4>(str);
                REQUIRE(str == " FEDCBA9876543210 FEDCBA98765432103210BA987676543210FEDCBA98"sv);
            }
        }
    }

    TEST_CASE("hexer", "[int_to_hex]" ) {
        {
            uint16_t i16 { 0xcafe };
            Bin::Int2Hex hex { i16 };

            {
                size_t pos { 1 };
                std::wstring str(6, L'*');
                hex.writeTo<>(str, pos);
                REQUIRE(str == L"*CAFE*"sv);
                hex.writeTo<>(str);
                REQUIRE(str == L"*CAFE*CAFE"sv);
                pos = 1;
                hex.writeTo<1>(str, pos);
                REQUIRE(str == L"*FEFE*CAFE"sv);
                hex.writeTo<1>(str);
                REQUIRE(str == L"*FEFE*CAFEFE"sv);
                pos = 3;
                hex.writeTo<1, 1>(str, pos);
                REQUIRE(str == L"*FECA*CAFEFE"sv);
                hex.writeTo<1, 1>(str);
                REQUIRE(str == L"*FECA*CAFEFECA"sv);
            }

            {
                size_t pos { 1 };
                std::string str(6, '*');
                hex.writeTo<>(str, pos);
                REQUIRE(str == "*CAFE*"sv);
                hex.writeTo<>(str);
                REQUIRE(str == "*CAFE*CAFE"sv);
                pos = 1;
                hex.writeTo<1>(str, pos);
                REQUIRE(str == "*FEFE*CAFE"sv);
                hex.writeTo<1>(str);
                REQUIRE(str == "*FEFE*CAFEFE"sv);
                pos = 3;
                hex.writeTo<1, 1>(str, pos);
                REQUIRE(str == "*FECA*CAFEFE"sv);
                hex.writeTo<1, 1>(str);
                REQUIRE(str == "*FECA*CAFEFECA"sv);
            }

            {
                std::wstring str { L"Test" };
                hex.rewriteIn(str, 5);
                REQUIRE(str == L"Test CAFE"sv);
                hex.appendTo(str);
                REQUIRE(str == L"Test CAFECAFE"sv);
                hex.rewriteIn(str, 5, 1);
                REQUIRE(str == L"Test FEFECAFE"sv);
                hex.appendTo(str, 1);
                REQUIRE(str == L"Test FEFECAFEFE"sv);
                hex.rewriteIn(str, 7, 1, 1);
                REQUIRE(str == L"Test FECACAFEFE"sv);
                hex.appendTo(str, 1, 1);
                REQUIRE(str == L"Test FECACAFEFECA"sv);
            }

            {
                std::string str { "Test" };
                hex.rewriteIn(str, 5);
                REQUIRE(str == "Test CAFE"sv);
                hex.appendTo(str);
                REQUIRE(str == "Test CAFECAFE"sv);
                hex.rewriteIn(str, 5, 1);
                REQUIRE(str == "Test FEFECAFE"sv);
                hex.appendTo(str, 1);
                REQUIRE(str == "Test FEFECAFEFE"sv);
                hex.rewriteIn(str, 7, 1, 1);
                REQUIRE(str == "Test FECACAFEFE"sv);
                hex.appendTo(str, 1, 1);
                REQUIRE(str == "Test FECACAFEFECA"sv);
            }
        }

        {
            uint64_t i64 { 0xfedc'ba98'7654'3210 };
            Bin::Int2Hex hex { i64 };

            {
                size_t pos { 1 };
                std::wstring str(18, L' ');
                hex.writeTo<>(str, pos);
                REQUIRE(str == L" FEDCBA9876543210 "sv);
                hex.writeTo<>(str);
                REQUIRE(str == L" FEDCBA9876543210 FEDCBA9876543210"sv);
                pos = 1;
                hex.writeTo<2>(str, pos);
                REQUIRE(str == L" 3210BA9876543210 FEDCBA9876543210"sv);
                hex.writeTo<2>(str);
                REQUIRE(str == L" 3210BA9876543210 FEDCBA98765432103210"sv);
                pos = 1;
                hex.writeTo<3, 3>(str, pos);
                REQUIRE(str == L" BA98769876543210 FEDCBA98765432103210"sv);
                hex.writeTo<3, 3>(str);
                REQUIRE(str == L" BA98769876543210 FEDCBA98765432103210BA9876"sv);
                pos = 1;
                hex.writeTo<4>(str, pos);
                REQUIRE(str == L" 7654321076543210 FEDCBA98765432103210BA9876"sv);
                hex.writeTo<4>(str);
                REQUIRE(str == L" 7654321076543210 FEDCBA98765432103210BA987676543210"sv);
                pos = 1;
                hex.writeTo<4, 4>(str, pos);
                REQUIRE(str == L" FEDCBA9876543210 FEDCBA98765432103210BA987676543210"sv);
                hex.writeTo<4, 4>(str);
                REQUIRE(str == L" FEDCBA9876543210 FEDCBA98765432103210BA987676543210FEDCBA98"sv);
            }

            {
                size_t pos { 1 };
                std::string str(18, ' ');
                hex.writeTo<>(str, pos);
                REQUIRE(str == " FEDCBA9876543210 "sv);
                hex.writeTo<>(str);
                REQUIRE(str == " FEDCBA9876543210 FEDCBA9876543210"sv);
                pos = 1;
                hex.writeTo<2>(str, pos);
                REQUIRE(str == " 3210BA9876543210 FEDCBA9876543210"sv);
                hex.writeTo<2>(str);
                REQUIRE(str == " 3210BA9876543210 FEDCBA98765432103210"sv);
                pos = 1;
                hex.writeTo<3, 3>(str, pos);
                REQUIRE(str == " BA98769876543210 FEDCBA98765432103210"sv);
                hex.writeTo<3, 3>(str);
                REQUIRE(str == " BA98769876543210 FEDCBA98765432103210BA9876"sv);
                pos = 1;
                hex.writeTo<4>(str, pos);
                REQUIRE(str == " 7654321076543210 FEDCBA98765432103210BA9876"sv);
                hex.writeTo<4>(str);
                REQUIRE(str == " 7654321076543210 FEDCBA98765432103210BA987676543210"sv);
                pos = 1;
                hex.writeTo<4, 4>(str, pos);
                REQUIRE(str == " FEDCBA9876543210 FEDCBA98765432103210BA987676543210"sv);
                hex.writeTo<4, 4>(str);
                REQUIRE(str == " FEDCBA9876543210 FEDCBA98765432103210BA987676543210FEDCBA98"sv);
            }

            {
                std::wstring str { L"Test" };
                hex.rewriteIn(str, 5);
                REQUIRE(str == L"Test FEDCBA9876543210"sv);
                hex.appendTo(str);
                REQUIRE(str == L"Test FEDCBA9876543210FEDCBA9876543210"sv);
                hex.rewriteIn(str, 5, 2);
                REQUIRE(str == L"Test 3210BA9876543210FEDCBA9876543210"sv);
                hex.appendTo(str, 2);
                REQUIRE(str == L"Test 3210BA9876543210FEDCBA98765432103210"sv);
                hex.rewriteIn(str, 5, 3, 3);
                REQUIRE(str == L"Test BA98769876543210FEDCBA98765432103210"sv);
                hex.appendTo(str, 3, 3);
                REQUIRE(str == L"Test BA98769876543210FEDCBA98765432103210BA9876"sv);
                hex.rewriteIn(str, 5, 4);
                REQUIRE(str == L"Test 7654321076543210FEDCBA98765432103210BA9876"sv);
                hex.appendTo(str, 4);
                REQUIRE(str == L"Test 7654321076543210FEDCBA98765432103210BA987676543210"sv);
                hex.rewriteIn(str, 5, 4, 4);
                REQUIRE(str == L"Test FEDCBA9876543210FEDCBA98765432103210BA987676543210"sv);
                hex.appendTo(str, 4, 4);
                REQUIRE(str == L"Test FEDCBA9876543210FEDCBA98765432103210BA987676543210FEDCBA98"sv);
            }

            {
                std::string str { "Test" };
                hex.rewriteIn(str, 5);
                REQUIRE(str == "Test FEDCBA9876543210"sv);
                hex.appendTo(str);
                REQUIRE(str == "Test FEDCBA9876543210FEDCBA9876543210"sv);
                hex.rewriteIn(str, 5, 2);
                REQUIRE(str == "Test 3210BA9876543210FEDCBA9876543210"sv);
                hex.appendTo(str, 2);
                REQUIRE(str == "Test 3210BA9876543210FEDCBA98765432103210"sv);
                hex.rewriteIn(str, 5, 3, 3);
                REQUIRE(str == "Test BA98769876543210FEDCBA98765432103210"sv);
                hex.appendTo(str, 3, 3);
                REQUIRE(str == "Test BA98769876543210FEDCBA98765432103210BA9876"sv);
                hex.rewriteIn(str, 5, 4);
                REQUIRE(str == "Test 7654321076543210FEDCBA98765432103210BA9876"sv);
                hex.appendTo(str, 4);
                REQUIRE(str == "Test 7654321076543210FEDCBA98765432103210BA987676543210"sv);
                hex.rewriteIn(str, 5, 4, 4);
                REQUIRE(str == "Test FEDCBA9876543210FEDCBA98765432103210BA987676543210"sv);
                hex.appendTo(str, 4, 4);
                REQUIRE(str == "Test FEDCBA9876543210FEDCBA98765432103210BA987676543210FEDCBA98"sv);
            }
        }
    }
}
