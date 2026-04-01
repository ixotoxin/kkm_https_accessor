// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "winstrapi.h"
#include "strings.h"
#if WITH_RELSL && defined(_MSC_VER) && !defined(__clang__)
#   include <cmake/variables.h>
#endif
#include <algorithm>
#include <array>

#if !WITH_RELSL || !defined(_MSC_VER) || defined(__clang__)
#   include <source_location>
#endif

namespace SrcLoc {
#if WITH_RELSL && defined(_MSC_VER) && !defined(__clang__)
    // ISSUE: Оптимизировать, если возможно.
    class Point {
        size_t m_line {};
        std::array<char, MAX_PATH> m_file {};

        static constexpr std::string_view c_prefix { BUILD_PREFIX };
        static constexpr std::string_view c_invalidPath { "[invalid-path]" };

    public:
        Point() = default;
        Point(const Point &) = default;
        Point(Point &&) = default;
        ~Point() = default;

        Point & operator=(const Point &) = default;
        Point & operator=(Point &&) noexcept = default;

        [[nodiscard]]
        static consteval Point current(
            const size_t line = __builtin_LINE(),
            const char * file = __builtin_FILE()
        ) noexcept {
            Point result;
            result.m_line = line;
            if (const std::string_view view0 { file }; view0.starts_with(c_prefix)) {
                std::ranges::copy(c_invalidPath, result.m_file.begin());
                result.m_file[c_invalidPath.size()] = 0;
            } else {
                const std::string_view view { view0.substr(c_prefix.length()) };
                if (view.length() >= MAX_PATH - 1 || view.empty()) {
                    std::ranges::copy(c_invalidPath, result.m_file.begin());
                    result.m_file[c_invalidPath.size()] = 0;
                } else {
                    result.m_file[0] = '.';
                    std::ranges::copy(view, result.m_file.begin() + 1);
                    result.m_file[view.size() + 1] = 0;
                }
            }
            return result;
        }

        [[nodiscard, maybe_unused]]
        constexpr size_t line() const noexcept {
            return m_line;
        }

        [[nodiscard, maybe_unused]]
        constexpr const char * file_name() const noexcept {
            return &m_file[0];
        }
    };
#else
    using Point = std::source_location;
#endif
}
