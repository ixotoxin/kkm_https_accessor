// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <log2/logger.h>

namespace Http {
    using Log::LoggerPtr;

    class Logger : public Log::CategoryLogger {
    public:
        Logger() = delete;
        Logger(const Logger &) noexcept = default;
        Logger(Logger &&) noexcept = default;

        explicit Logger(std::wstring && prefix) noexcept
        : CategoryLogger(Log::Category::WebServer, std::move(prefix)) {}

        explicit Logger(const std::wstring_view prefix = {}) noexcept
        : CategoryLogger(Log::Category::WebServer, prefix) {}

        explicit Logger(CategoryLogger & parent, const std::wstring_view prefix = {}) noexcept // NOLINT
        : CategoryLogger(parent, Log::Category::WebServer, prefix) {}

        ~Logger() override = default;

        Logger & operator=(const Logger &) = delete;
        Logger & operator=(Logger &&) = delete;
    };
}
