// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "wconv.h"
#include <algorithm>
#include <string>
#include <format>

namespace Basic {
    template<class T>
    requires std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>
    struct Message {
        T m_message {};
        size_t m_reserveSize;

        Message() = delete;

        explicit Message(const size_t size)
        : m_reserveSize { size } {
            assert(m_reserveSize >= 31);
            m_message.reserve(m_reserveSize);
        }

        Message(const Message &) = delete;
        Message(Message &&) = delete;
        ~Message() = default;

        Message & operator=(const Message &) = delete;
        Message & operator=(Message &&) = delete;
    };

    namespace Wcs {
        using Message = Message<std::wstring>;

        template<size_t S = 0>
        struct Cat : Message {
            template<typename ... Args>
            requires (sizeof...(Args) > 1) && (std::is_convertible_v<Args, std::wstring_view> && ...)
            Cat(Args && ... args) // NOLINT
            : Message(S) {
                (m_message.append(args), ...);
            }

            template<typename ... Args>
            requires (sizeof...(Args) > 1) && (std::is_convertible_v<Args, std::wstring_view> && ...)
            Cat(const size_t size, Args && ... args) // NOLINT
            : Message(std::max(S, size)) {
                (m_message.append(args), ...);
            }

            template<typename ... Args>
            requires (sizeof...(Args) > 1) && (std::is_convertible_v<Args, std::string_view> && ...)
            Cat(Args && ... args) // NOLINT
            : Message(S) {
                std::string temp {};
                temp.reserve(m_reserveSize);
                (temp.append(args), ...);
                Text::convert(m_message, temp);
            }

            template<typename ... Args>
            requires (sizeof...(Args) > 1) && (std::is_convertible_v<Args, std::string_view> && ...)
            Cat(const size_t size, Args && ... args) // NOLINT
            : Message(std::max(S, size)) {
                std::string temp {};
                temp.reserve(m_reserveSize);
                (temp.append(args), ...);
                Text::convert(m_message, temp);
            }

            Cat(const Cat &) = delete;
            Cat(Cat &&) = delete;
            ~Cat() = default;

            Cat & operator=(const Cat &) = delete;
            Cat & operator=(Cat &&) = delete;
        };

        template<size_t S = 0>
        struct Fmt : Message {
            template<typename ... Args>
            requires (sizeof...(Args) > 0)
            Fmt(const std::wstring_view fmt, Args && ... args) // NOLINT
            : Message(S) {
                std::vformat_to(std::back_inserter(m_message), fmt, std::make_wformat_args(args...));
            }

            template<typename ... Args>
            requires (sizeof...(Args) > 0)
            Fmt(const size_t size, const std::wstring_view fmt, Args && ... args)
            : Message(std::max(S, size)) {
                std::vformat_to(std::back_inserter(m_message), fmt, std::make_wformat_args(args...));
            }

            template<typename ... Args>
            requires (sizeof...(Args) > 0)
            Fmt(const std::string_view fmt, Args && ... args) // NOLINT
            : Message(S) {
                std::string temp {};
                temp.reserve(m_reserveSize);
                std::vformat_to(std::back_inserter(temp), fmt, std::make_format_args(args...));
                Text::convert(m_message, temp);
            }

            template<typename ... Args>
            requires (sizeof...(Args) > 0)
            Fmt(const size_t size, const std::string_view fmt, Args && ... args)
            : Message(std::max(S, size)) {
                std::string temp {};
                temp.reserve(m_reserveSize);
                std::vformat_to(std::back_inserter(temp), fmt, std::make_format_args(args...));
                Text::convert(m_message, temp);
            }

            Fmt(const Fmt &) = delete;
            Fmt(Fmt &&) = delete;
            ~Fmt() = default;

            Fmt & operator=(const Fmt &) = delete;
            Fmt & operator=(Fmt &&) = delete;
        };
    }

    namespace Mbs {
        using Message = Message<std::string>;

        template<size_t S = 0>
        struct Cat : Message {
            template<typename ... Args>
            requires (sizeof...(Args) > 1) && (std::is_convertible_v<Args, std::string_view> && ...)
            Cat(Args && ... args) // NOLINT
            : Message(S) {
                (m_message.append(args), ...);
            }

            template<typename ... Args>
            requires (sizeof...(Args) > 1) && (std::is_convertible_v<Args, std::string_view> && ...)
            Cat(const size_t size, Args && ... args) // NOLINT
            : Message(std::max(S, size)) {
                (m_message.append(args), ...);
            }

            template<typename ... Args>
            requires (sizeof...(Args) > 1) && (std::is_convertible_v<Args, std::wstring_view> && ...)
            Cat(Args && ... args) // NOLINT
            : Message(S) {
                std::wstring temp {};
                temp.reserve(m_reserveSize);
                (temp.append(args), ...);
                Text::convert(m_message, temp);
            }

            template<typename ... Args>
            requires (sizeof...(Args) > 1) && (std::is_convertible_v<Args, std::wstring_view> && ...)
            Cat(const size_t size, Args && ... args) // NOLINT
            : Message(std::max(S, size)) {
                std::wstring temp {};
                temp.reserve(m_reserveSize);
                (temp.append(args), ...);
                Text::convert(m_message, temp);
            }

            Cat(const Cat &) = delete;
            Cat(Cat &&) = delete;
            ~Cat() = default;

            Cat & operator=(const Cat &) = delete;
            Cat & operator=(Cat &&) = delete;
        };

        template<size_t S = 0>
        struct Fmt : Message {
            template<typename ... Args>
            requires (sizeof...(Args) > 0)
            Fmt(const std::string_view fmt, Args && ... args) // NOLINT
            : Message(S) {
                std::vformat_to(std::back_inserter(m_message), fmt, std::make_format_args(args...));
            }

            template<typename ... Args>
            requires (sizeof...(Args) > 0)
            Fmt(const size_t size, const std::string_view fmt, Args && ... args)
            : Message(std::max(S, size)) {
                std::vformat_to(std::back_inserter(m_message), fmt, std::make_format_args(args...));
            }

            template<typename ... Args>
            requires (sizeof...(Args) > 0)
            Fmt(const std::wstring_view fmt, Args && ... args) // NOLINT
            : Message(S) {
                std::wstring temp {};
                temp.reserve(m_reserveSize);
                std::vformat_to(std::back_inserter(temp), fmt, std::make_wformat_args(args...));
                Text::convert(m_message, temp);
            }

            template<typename ... Args>
            requires (sizeof...(Args) > 0)
            Fmt(const size_t size, const std::wstring_view fmt, Args && ... args)
            : Message(std::max(S, size)) {
                std::wstring temp {};
                temp.reserve(m_reserveSize);
                std::vformat_to(std::back_inserter(temp), fmt, std::make_wformat_args(args...));
                Text::convert(m_message, temp);
            }

            Fmt(const Fmt &) = delete;
            Fmt(Fmt &&) = delete;
            ~Fmt() = default;

            Fmt & operator=(const Fmt &) = delete;
            Fmt & operator=(Fmt &&) = delete;
        };
    }
}
