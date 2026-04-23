// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "errexp.h"
#include "defer.h"

namespace System {
    namespace {
        void cleanUpMessage(wchar_t * text) {
            wchar_t * end { text + std::wcslen(text) };
            if (end != text) {
                --end;
                while (
                    end != text
                    && (*end == L' ' || *end == L'\n' || *end == L'\r' || *end == L'\t' || *end == L'\v' || *end == L'\f')
                ) {
                    *end-- = L'\0';
                }
                while (--end != text) {
                    if (*end == L'\n' || *end == L'\r' /*|| *end == L'\t' || *end == L'\v' || *end == L'\f'*/) {
                        *end = L' ';
            } } }
        }
    }

    [[nodiscard, maybe_unused]]
    std::wstring errorMessage(const ::DWORD error) noexcept {
        wchar_t * text { nullptr };
        const Deferred::LocalFree freeTextBuffer { text };
        try {
            const auto status
                = ::FormatMessageW(
                    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
                    nullptr,
                    error,
                    LANG_SYSTEM_DEFAULT, // MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    reinterpret_cast<::LPWSTR>(&text),
                    0,
                    nullptr
                );
            if (status) {
                cleanUpMessage(text);
            } else {
                return { Basic::Wcs::c_somethingWrong.data(), Basic::Wcs::c_somethingWrong.size() };
            }
        } catch (...) {
            return { Basic::Wcs::c_somethingWrong.data(), Basic::Wcs::c_somethingWrong.size() };
        }
        return text;
    }
}
