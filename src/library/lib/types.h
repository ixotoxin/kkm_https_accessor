// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

consteval size_t operator""_size_t(const unsigned long long value) noexcept { return value; }

namespace Basic {
    template<typename T, typename U>
    struct RefPair {
        using FirstType = T;
        using SecondType = U;

        T & m_first;
        U & m_second;

        RefPair() = delete;
        RefPair(const RefPair &) = delete;
        RefPair(RefPair &&) = delete;
        RefPair(T & first, U & second) : m_first { first }, m_second { second } {}
        ~RefPair() = default;

        RefPair & operator=(const RefPair &) = delete;
        RefPair & operator=(RefPair &&) = delete;
    };
}
