// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "winapi.h"
#include <concepts>
#include <utility>
#include <functional>

namespace Deferred {
    template<std::invocable T>
    class Exec {
        T m_func;
        std::exception_ptr * m_exception { nullptr };
        bool m_permitted { true };

    public:
        Exec() = delete;
        Exec(const Exec &) = delete;
        Exec(Exec &&) = delete;

        [[maybe_unused]]
        explicit Exec(const T & func) noexcept
        : m_func { func } {}

        [[maybe_unused]]
        explicit Exec(T && func) noexcept
        : m_func { std::forward<T>(func) } {}

        [[maybe_unused]]
        explicit Exec(const T & func, std::exception_ptr & exception) noexcept
        : m_func { func }, m_exception { &exception } {}

        [[maybe_unused]]
        explicit Exec(T && func, std::exception_ptr & exception) noexcept
        : m_func { std::forward<T>(func) }, m_exception { &exception } {}

        ~Exec() {
            if (m_permitted) {
                try {
                    std::invoke(m_func);
                } catch (...) {
                    if (m_exception) {
                        *m_exception = std::current_exception();
                    }
                }
            }
        }

        Exec & operator=(const Exec &) = delete;
        Exec & operator=(Exec &&) = delete;

        [[maybe_unused]]
        void perform(const bool repeatable = false) noexcept {
            if (m_permitted) {
                try {
                    std::invoke(m_func);
                } catch (...) {
                    if (m_exception) {
                        *m_exception = std::current_exception();
                    }
                }
                m_permitted = repeatable;
            }
        }

        [[maybe_unused]]
        void cancel() noexcept {
            m_permitted = false;
        }
    };

    class LocalFree {
        HLOCAL & m_memory;
        bool m_permitted { true };

    public:
        LocalFree() = delete;
        LocalFree(const LocalFree &) = delete;
        LocalFree(LocalFree &&) = delete;

        [[maybe_unused]]
        explicit LocalFree(auto & memory) noexcept
        : m_memory(reinterpret_cast<HLOCAL &>(memory)) {}

        ~LocalFree() {
            perform();
        }

        LocalFree & operator=(const LocalFree &) = delete;
        LocalFree & operator=(LocalFree &&) = delete;

        [[maybe_unused]]
        void perform() const noexcept {
            if (m_permitted && m_memory) {
                ::LocalFree(m_memory);
                m_memory = nullptr;
            }
        }

        [[maybe_unused]]
        void cancel() noexcept {
            m_permitted = false;
        }
    };
}
