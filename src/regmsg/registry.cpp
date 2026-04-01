// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "registry.h"
#include <lib/defer.h>
#include <lib/errexp.h>
#include <lib/except.h>
#include <sddl.h>
#include <winreg.h>
#include <memory>

namespace Registry {
    using Basic::Failure;

    constexpr size_t c_maxValueSize = 65'536;

    constexpr std::wstring_view c_securityDescriptor {
        L"D:"                // Discretionary ACL
        L"(D;OICI;GA;;;BG)"  // Deny access to Built-in Guests
        L"(D;OICI;GA;;;AN)"  // Deny access to Anonymous logon
        L"(A;OICI;KR;;;AU)"  // Allow KEY_READ to Authenticated Users
        L"(A;OICI;KA;;;LS)"  // Allow KEY_ALL_ACCESS to Local Service account
        L"(A;OICI;KA;;;BA)"  // Allow KEY_ALL_ACCESS to Built-in Administrators
    };

    void Key::createKey(HKEY key, const std::wstring_view subKey) {
        ::SECURITY_ATTRIBUTES securityAttributes {};
        Deferred::LocalFree deferredLocalFree(securityAttributes.lpSecurityDescriptor);
        securityAttributes.nLength = sizeof(::SECURITY_ATTRIBUTES);
        securityAttributes.bInheritHandle = false;
        const auto success
            = ::ConvertStringSecurityDescriptorToSecurityDescriptorW(
                c_securityDescriptor.data(),
                SDDL_REVISION_1,
                &securityAttributes.lpSecurityDescriptor,
                nullptr
            );
        if (!success) {
            throw Failure(System::explainError(L"ConvertStringSecurityDescriptorToSecurityDescriptorW(...)")); // NOLINT(*-exception-baseclass)
        }
        const auto status = ::RegCreateKeyExW(
            key,
            subKey.data(),
            0,
            nullptr,
            REG_OPTION_NON_VOLATILE,
            KEY_ALL_ACCESS,
            &securityAttributes,
            &m_key,
            nullptr
        );
        if (status != ERROR_SUCCESS) {
            throw Failure(System::explainError(L"RegCreateKeyExW(...)", status)); // NOLINT(*-exception-baseclass)
        }
    }

    Key::~Key() {
        if (m_key) {
            ::RegCloseKey(m_key);
        }
    }

    [[nodiscard, maybe_unused]]
    ::DWORD Key::getNumber(const std::wstring_view name) const {
        ::DWORD type = REG_DWORD;
        ::DWORD result;
        ::DWORD size = sizeof(::DWORD);
        const auto status
            = ::RegQueryValueExW(m_key, name.data(), nullptr, &type, reinterpret_cast<LPBYTE>(&result), &size);
        if (status != ERROR_SUCCESS) {
            throw Failure(System::explainError(L"RegQueryValueExW(...)", status)); // NOLINT(*-exception-baseclass)
        }
        return result;
    }

    [[maybe_unused]]
    void Key::setNumber(const std::wstring_view name, ::DWORD value) const {
        const auto status
            = ::RegSetValueExW(m_key, name.data(), 0, REG_DWORD, reinterpret_cast<LPBYTE>(&value), sizeof(value));
        if (status != ERROR_SUCCESS) {
            throw Failure(System::explainError(L"RegSetValueExW(...)", status)); // NOLINT(*-exception-baseclass)
        }
    }

    [[nodiscard, maybe_unused]]
    bool Key::tryGetNumber(const std::wstring_view name, ::DWORD & value) const {
        ::DWORD type = REG_DWORD;
        ::DWORD buffer;
        ::DWORD size = sizeof(buffer);
        const auto status
            = ::RegQueryValueExW(m_key, name.data(), nullptr, &type, reinterpret_cast<LPBYTE>(&buffer), &size);
        if (status == ERROR_SUCCESS) {
            value = buffer;
            return true;
        }
        return false;
    }

    [[nodiscard, maybe_unused]]
    bool Key::trySetNumber(const std::wstring_view name, ::DWORD value) const {
        const auto status
            = ::RegSetValueExW(m_key, name.data(), 0, REG_DWORD, reinterpret_cast<LPBYTE>(&value), sizeof(value));
        return status == ERROR_SUCCESS;
    }

    [[nodiscard, maybe_unused]]
    std::wstring Key::getString(const std::wstring_view name) const {
        ::DWORD type = REG_EXPAND_SZ;
        wchar_t buffer[c_maxValueSize];
        ::DWORD size = sizeof(buffer);
        const auto status
            = ::RegQueryValueExW(m_key, name.data(), nullptr, &type, reinterpret_cast<LPBYTE>(buffer), &size);
        if (status != ERROR_SUCCESS) {
            throw Failure(System::explainError(L"RegQueryValueExW(...)", status)); // NOLINT(*-exception-baseclass)
        }
        if (size > 0) {
            return { buffer };
        }
        return {};
    }

    [[maybe_unused]]
    void Key::setString(const std::wstring_view name, const std::wstring_view value) const {
        const auto status
            = ::RegSetValueExW(
                m_key,
                name.data(),
                0,
                REG_EXPAND_SZ,
                reinterpret_cast<LPBYTE>(const_cast<wchar_t *>(value.data())),
                static_cast<DWORD>(value.length() * sizeof(wchar_t))
            );
        if (status != ERROR_SUCCESS) {
            throw Failure(System::explainError(L"RegSetValueExW(...)", status)); // NOLINT(*-exception-baseclass)
        }
    }

    [[nodiscard, maybe_unused]]
    bool Key::tryGetString(const std::wstring_view name, std::wstring & value) const {
        ::DWORD type = REG_EXPAND_SZ;
        wchar_t buffer[c_maxValueSize];
        ::DWORD size = sizeof(buffer);
        const auto status
            = ::RegQueryValueExW(m_key, name.data(), nullptr, &type, reinterpret_cast<LPBYTE>(buffer), &size);
        if (status == ERROR_SUCCESS) {
            if (size > 0) {
                value.assign(buffer);
            } else {
                value.clear();
            }
            return true;
        }
        return false;
    }

    [[nodiscard, maybe_unused]]
    bool Key::trySetString(const std::wstring_view name, const std::wstring_view value) const {
        const auto status
            = ::RegSetValueExW(
                m_key, name.data(), 0, REG_EXPAND_SZ,
                reinterpret_cast<LPBYTE>(const_cast<wchar_t *>(value.data())),
                value.length() * sizeof(wchar_t)
            );
        return status == ERROR_SUCCESS;
    }

    [[nodiscard, maybe_unused]]
    std::pair<LSTATUS, std::wstring_view> Key::realWalk(const WalkerType & walker) const {
        ::DWORD values;
        ::DWORD maxValueNameSize;
        ::DWORD maxValueSize;
        auto status
            = ::RegQueryInfoKeyW(
                m_key, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                &values, &maxValueNameSize, &maxValueSize, nullptr, nullptr
            );
        if (status != ERROR_SUCCESS) {
            return { status, L"RegQueryInfoKeyW(...)" };
        }
        ++maxValueNameSize;
        ++maxValueSize;
        const auto valueName = std::make_shared<wchar_t[]>(maxValueNameSize);
        ::DWORD valueNameSize;
        ::DWORD valueType;
        const auto value = std::make_shared_for_overwrite<BYTE[]>(maxValueSize);
        ::DWORD valueSize;
        for (::INT_PTR i = 0; i < values; ++i) {
            valueNameSize = maxValueNameSize;
            valueSize = maxValueSize;
            status = ::RegEnumValueW(
                m_key, i, valueName.get(), &valueNameSize, nullptr, &valueType, value.get(), &valueSize
            );
            if (status != ERROR_SUCCESS) {
                return { status, L"RegEnumValueW(...)" };
            }
            if (valueType == REG_SZ || valueType == REG_EXPAND_SZ) {
                if (valueSize > 0) {
                    walker(
                        std::wstring_view(valueName.get(), valueNameSize),
                        ValueType(std::in_place_type<std::wstring>, reinterpret_cast<wchar_t *>(value.get()))
                    );
                } else {
                    walker(std::wstring_view(valueName.get(), valueNameSize), ValueType(std::wstring()));
                }
                valueSize = (valueSize / sizeof(wchar_t)) - 1;
            } else if (valueType == REG_DWORD) {
                walker(
                    std::wstring_view(valueName.get(), valueNameSize),
                    ValueType(std::in_place_type<DWORD>, *reinterpret_cast<DWORD *>(value.get()))
                );
            }
        }
        return { ERROR_SUCCESS, L"" };
    }

    [[maybe_unused]]
    void Key::walk(const WalkerType & walker) const {
        auto [status, func] = realWalk(walker);
        if (status != ERROR_SUCCESS) {
            throw Failure(System::explainError(func, status)); // NOLINT(*-exception-baseclass)
        }
    }

    [[nodiscard, maybe_unused]]
    bool Key::tryWalk(const WalkerType & walker) const {
        return realWalk(walker).first == ERROR_SUCCESS;
    }

    NewKey::NewKey(HKEY key, const std::wstring_view subKey) : Key() {
        createKey(key, subKey);
    }

    RoKey::RoKey(HKEY key, const std::wstring_view subKey) : Key() {
        const auto status = ::RegOpenKeyExW(key, subKey.data(), 0, KEY_READ, &m_key);
        if (status != ERROR_SUCCESS) {
            throw Failure(System::explainError(L"RegOpenKeyExW(...)", status)); // NOLINT(*-exception-baseclass)
        }
    }

    RwKey::RwKey(HKEY key, const std::wstring_view subKey, const REGSAM samDesired) : Key() {
        const auto status = ::RegOpenKeyExW(key, subKey.data(), 0, samDesired, &m_key);
        if (status != ERROR_SUCCESS) {
            createKey(key, subKey);
        }
    }
}
