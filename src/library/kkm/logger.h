// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <log2/logger.h>

namespace Kkm {
    using Log::LoggerPtr;

    class Logger : public Log::CategoryLogger {
    public:
        Logger() = delete;
        Logger(const Logger &) noexcept = default;
        Logger(Logger &&) noexcept = default;

        explicit Logger(std::wstring && prefix) noexcept
        : CategoryLogger(Log::Category::CashRegister, std::move(prefix)) {}

        explicit Logger(const std::wstring_view prefix = {}) noexcept
        : CategoryLogger(Log::Category::CashRegister, prefix) {}

        explicit Logger(CategoryLogger & parent, const std::wstring_view prefix = {}) noexcept // NOLINT
        : CategoryLogger(parent, Log::Category::CashRegister, prefix) {}

        ~Logger() override = default;

        Logger & operator=(const Logger &) = delete;
        Logger & operator=(Logger &&) = delete;
    };

    template<Meta::View T, typename ... Args>
    [[maybe_unused]]
    void log(const Log::Level level, const T message, Args && ... args) noexcept {
        Log::write<typename Meta::TextTrait<T>::View>(
            Log::Category::CashRegister, level, {}, message, std::forward<Args>(args)...
        );
    }

    template<Meta::Char T, typename ... Args>
    [[maybe_unused]]
    void log(const Log::Level level, const T * message, Args && ... args) noexcept {
        Log::write<typename Meta::TextTrait<T>::View>(
            Log::Category::CashRegister, level, {}, message, std::forward<Args>(args)...
        );
    }

    template<Meta::String T, typename ... Args>
    [[maybe_unused]]
    void log(const Log::Level level, const T & message, Args && ... args) noexcept {
        Log::write<typename Meta::TextTrait<T>::View>(
            Log::Category::CashRegister, level, {}, message, std::forward<Args>(args)...
        );
    }

    [[maybe_unused]]
    inline void log(const Log::Level level, const Basic::Failure & e) noexcept {
        Log::write(Log::Category::CashRegister, level, {}, e);
    }
}
