// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/winapi.h>
#include <variant>
#include <functional>
#include <string>

namespace Registry {
    using ValueType = std::variant<::DWORD, std::wstring>;
    using WalkerType = std::function<void(std::wstring_view, const ValueType &)>;

    class Key {
    public:
        Key(const Key &) = delete;
        Key(Key &&) = delete;
        virtual ~Key();

        Key & operator=(const Key &) = delete;
        Key & operator=(Key &&) = delete;

        [[nodiscard, maybe_unused]] ::DWORD getNumber(std::wstring_view) const;
        [[maybe_unused]] void setNumber(std::wstring_view, ::DWORD) const;
        [[nodiscard, maybe_unused]] bool tryGetNumber(std::wstring_view, ::DWORD &) const;
        [[nodiscard, maybe_unused]] bool trySetNumber(std::wstring_view, ::DWORD) const;

        [[nodiscard, maybe_unused]] std::wstring getString(std::wstring_view) const;
        [[maybe_unused]] void setString(std::wstring_view, std::wstring_view) const;
        [[nodiscard, maybe_unused]] bool tryGetString(std::wstring_view, std::wstring &) const;
        [[nodiscard, maybe_unused]] bool trySetString(std::wstring_view, std::wstring_view) const;

        [[maybe_unused]] void walk(const WalkerType &) const;
        [[nodiscard, maybe_unused]] bool tryWalk(const WalkerType &) const;

    protected:
        HKEY m_key {};

        Key() = default;

        [[nodiscard, maybe_unused]] std::pair<LSTATUS, std::wstring_view> realWalk(const WalkerType &) const;
        void createKey(HKEY, std::wstring_view);
    };

    class NewKey final : public Key {
    public:
        NewKey() = delete;
        NewKey(const NewKey &) = delete;
        NewKey(NewKey &&) = delete;
        NewKey(HKEY, std::wstring_view);
        ~NewKey() override = default;

        NewKey & operator=(const NewKey &) = delete;
        NewKey & operator=(NewKey &&) = delete;
    };

    class RoKey final : public Key {
    public:
        RoKey() = delete;
        RoKey(const RoKey &) = delete;
        RoKey(RoKey &&) = delete;
        RoKey(HKEY, std::wstring_view/*, REGSAM = KEY_READ*/);
        ~RoKey() override = default;

        RoKey & operator=(const RoKey &) = delete;
        RoKey & operator=(RoKey &&) = delete;
    };

    class RwKey final : public Key {
    public:
        RwKey() = delete;
        RwKey(const RwKey &) = delete;
        RwKey(RwKey &&) = delete;
        RwKey(HKEY, std::wstring_view, REGSAM = KEY_ALL_ACCESS);
        ~RwKey() override = default;

        RwKey & operator=(const RwKey &) = delete;
        RwKey & operator=(RwKey &&) = delete;
    };
}
