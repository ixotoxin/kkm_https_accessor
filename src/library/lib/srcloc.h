// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <cmake/options.h>
#if WITH_RELSL && defined(_MSC_VER) && !defined(__clang__)
#   include <cmake/variables.h>
#endif
#include "winstrapi.h"
#include "strings.h"
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
            std::string fixedPrefix { c_prefix };
            std::ranges::replace(fixedPrefix, '/', '\\');
            if (!fixedPrefix.ends_with('\\')) {
                fixedPrefix.append("\\");
            }
            std::string fixedFile { file };
            std::ranges::replace(fixedFile, '/', '\\');
            if (fixedFile.starts_with(fixedPrefix)) {
                std::string_view view { fixedFile };
                view.remove_prefix(fixedPrefix.size());
                if (view.empty() || view.length() >= MAX_PATH - 1) {
                    std::ranges::copy(c_invalidPath, result.m_file.begin());
                    result.m_file[c_invalidPath.size()] = 0;
                } else {
                    std::ranges::copy(view, result.m_file.begin());
                    result.m_file[view.size()] = 0;
                }
            } else {
                std::ranges::copy(c_invalidPath, result.m_file.begin());
                result.m_file[c_invalidPath.size()] = 0;
            }
            return result;
        }

        [[nodiscard, maybe_unused]]
        constexpr size_t line() const noexcept {
            return m_line;
        }

        [[nodiscard, maybe_unused]]
        constexpr const char * file_name() const noexcept {
            return m_file.data();
        }
    };
#else
    using Point = std::source_location;
#endif
}
