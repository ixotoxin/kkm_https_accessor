// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "meta.h"
#include "text_traits.h"
#include <bit>
#include <array>

namespace Bin {
    template<size_t S>
    requires (S > 0)
    struct Hexer {
        struct HexPair {
            uint8_t m_l:4;
            uint8_t m_h:4;
        };

        static constexpr size_t c_size { S };
        std::array<HexPair, c_size> m_bytes;

        template<size_t N = S, size_t O = 0>
        requires (N > 0) && (N + O <= S)
        [[maybe_unused]]
        void writeTo(Meta::String auto & output) const {
            // static_assert(N > 0);
            // static_assert(N + O <= S);
            using Txt = Meta::TextTrait<decltype(output)>;
            for (size_t count { N }, offset { O + N - 1 }; count; --count) {
                output.push_back(Txt::c_hexDigits[m_bytes[offset].m_h]);
                output.push_back(Txt::c_hexDigits[m_bytes[offset--].m_l]);
            }
        }

        template<size_t N = S, size_t O = 0>
        requires (N > 0) && (N + O <= S)
        [[maybe_unused]]
        void writeTo(Meta::String auto & output, size_t & pos) const {
            // static_assert(N > 0);
            // static_assert(N + O <= S);
            using Txt = Meta::TextTrait<decltype(output)>;
            if (output.size() < pos + N * 2) {
                output.resize(pos + N * 2, Txt::c_defaultPadding);
            }
            for (size_t count { N }, offset { O + N - 1 }; count; --count) {
                output[pos++] = Txt::c_hexDigits[m_bytes[offset].m_h];
                output[pos++] = Txt::c_hexDigits[m_bytes[offset--].m_l];
            }
        }

        [[maybe_unused]]
        void appendTo(Meta::String auto & output, size_t count = S, size_t offset = 0) const {
            assert(count > 0);
            assert(count + offset <= S);
            using Txt = Meta::TextTrait<decltype(output)>;
            offset += count - 1;
            while (count--) {
                output.push_back(Txt::c_hexDigits[m_bytes[offset].m_h]);
                output.push_back(Txt::c_hexDigits[m_bytes[offset--].m_l]);
            }
        }

        [[maybe_unused]]
        void rewriteIn(Meta::String auto & output, size_t pos, size_t count = S, size_t offset = 0) const {
            assert(count > 0);
            assert(count + offset <= S);
            using Txt = Meta::TextTrait<decltype(output)>;
            if (output.size() < pos + count * 2) {
                output.resize(pos + count * 2, Txt::c_defaultPadding);
            }
            offset += count - 1;
            while (count--) {
                output[pos++] = Txt::c_hexDigits[m_bytes[offset].m_h];
                output[pos++] = Txt::c_hexDigits[m_bytes[offset--].m_l];
            }
        }
    };

#if defined(_MSC_VER) && !defined(__clang__)
    static_assert(0xfe == std::bit_cast<uint8_t>(Hexer<42>::HexPair { 0xe, 0xf }));
#endif

    template<std::integral T>
    class Int2Hex final : Hexer<sizeof(T)> {
    public:
        Int2Hex() = delete;
        Int2Hex(const Int2Hex &) = default;
        Int2Hex(Int2Hex &&) = default;
        explicit Int2Hex(T value) : Hexer<sizeof(T)> { std::bit_cast<Hexer<sizeof(T)>>(value) } {}
        ~Int2Hex() = default;

        Int2Hex & operator=(const Int2Hex &) = default;
        Int2Hex & operator=(Int2Hex &&) noexcept = default;

        using Hexer<sizeof(T)>::c_size;
        using Hexer<sizeof(T)>::writeTo;
        using Hexer<sizeof(T)>::appendTo;
        using Hexer<sizeof(T)>::rewriteIn;
    };
}
