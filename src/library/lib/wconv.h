// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "winstrapi.h"
#include "strings.h"
#include <cmake/options.h>
#include <cassert>
#include <memory>
#include <string>

namespace Text {
    constexpr std::string::size_type c_minimalCapacity {
        std::max(sizeof(std::string), sizeof(std::wstring) / sizeof(wchar_t)) + 2
    };

    [[maybe_unused]]
    inline bool convert(std::wstring & output, const std::string_view text) noexcept try {
        if (text.empty()) {
            output.clear();
            return output.empty();
        }
        using st = std::wstring::size_type;
        const st estimatedSize = WIN_MB2WC_ESTIMATED(text.data(), text.size());
        if (estimatedSize <= 0) {
            return false;
        }
#if WITH_SBIAC
        const st requiredCapacity = std::max(estimatedSize, c_minimalCapacity);
        if (requiredCapacity > output.capacity()) {
            output.reserve(requiredCapacity);
        }
        output.resize(estimatedSize);
        const st estimatedSize2 = WIN_MB2WC(text.data(), text.size(), output.data(), output.capacity());
        assert(estimatedSize == estimatedSize2);
        return estimatedSize == estimatedSize2;
#else
        const auto buffer = std::make_unique_for_overwrite<wchar_t[]>(estimatedSize);
        const st size = WIN_MB2WC(text.data(), text.size(), buffer.get(), estimatedSize);
        assert(estimatedSize == size);
        if (size <= 0) {
            return false;
        }
        output.assign(buffer.get(), size);
        return !output.empty();
#endif
    } catch (...) {
        return false;
    }

    [[maybe_unused]]
    inline bool convert(std::string & output, const std::wstring_view text) noexcept try {
        if (text.empty()) {
            output.clear();
            return output.empty();
        }
        using st = std::wstring::size_type;
        const st estimatedSize = WIN_WC2MB_ESTIMATED(text.data(), text.size());
        if (estimatedSize <= 0) {
            return false;
        }
#if WITH_SBIAC
        const st requiredCapacity = std::max(estimatedSize + 1, c_minimalCapacity);
        if (requiredCapacity > output.capacity()) {
            output.reserve(requiredCapacity);
        }
        output.resize(estimatedSize);
        const st estimatedSize2 = WIN_WC2MB(text.data(), text.size(), output.data(), output.capacity());
        assert(estimatedSize == estimatedSize2);
        return estimatedSize == estimatedSize2;
#else
        const auto buffer = std::make_unique_for_overwrite<char[]>(estimatedSize);
        const st size = WIN_WC2MB(text.data(), text.size(), buffer.get(), estimatedSize);
        assert(estimatedSize == size);
        if (size <= 0) {
            return false;
        }
        output.assign(buffer.get(), size);
        return !output.empty();
#endif
    } catch (...) {
        return false;
    }

    [[nodiscard, maybe_unused]]
    inline std::wstring convert(const std::string_view text) noexcept {
        std::wstring result {};
        try {
            if (!convert(result, text)) {
                result.assign(Basic::Wcs::c_fallbackErrorMessage);
            }
        } catch (...) {
            result.assign(Basic::Wcs::c_fallbackErrorMessage);
        }
        return result;
    }

    [[nodiscard, maybe_unused]]
    inline std::string convert(const std::wstring_view text) noexcept {
        std::string result {};
        try {
            if (!convert(result, text)) {
                result.assign(Basic::Mbs::c_fallbackErrorMessage);
            }
        } catch (...) {
            result.assign(Basic::Mbs::c_fallbackErrorMessage);
        }
        return result;
    }

    [[maybe_unused]]
    inline bool appendConverted(std::wstring & output, const std::string_view text) noexcept try {
        if (text.empty()) {
            return false;
        }
        using st = std::wstring::size_type;
        const st additionalSize = WIN_MB2WC_ESTIMATED(text.data(), text.size());
        if (additionalSize <= 0) {
            return false;
        }
        const st initialSize = output.size();
        const st estimatedSize = initialSize + additionalSize;
        const st requiredCapacity = std::max(estimatedSize + 1, c_minimalCapacity);
        if (requiredCapacity > output.capacity()) {
            output.reserve(requiredCapacity);
        }
        output.resize(estimatedSize);
        const st additionalSize2 = WIN_MB2WC(text.data(), text.size(), output.data() + initialSize, requiredCapacity);
        assert(additionalSize == additionalSize2);
        return additionalSize == additionalSize2;
    } catch (...) {
        return false;
    }

    [[maybe_unused]]
    inline bool appendConverted(std::string & output, const std::wstring_view text) noexcept try {
        if (text.empty()) {
            return false;
        }
        using st = std::string::size_type;
        const st additionalSize = WIN_WC2MB_ESTIMATED(text.data(), text.size());
        if (additionalSize <= 0) {
            return false;
        }
        const st initialSize = output.size();
        const st estimatedSize = initialSize + additionalSize;
        const st requiredCapacity = std::max(estimatedSize + 1, c_minimalCapacity);
        if (requiredCapacity > output.capacity()) {
            output.reserve(requiredCapacity);
        }
        output.resize(estimatedSize);
        const st additionalSize2 = WIN_WC2MB(text.data(), text.size(), output.data() + initialSize, requiredCapacity);
        assert(additionalSize == additionalSize2);
        return additionalSize == additionalSize2;
    } catch (...) {
        return false;
    }
}
