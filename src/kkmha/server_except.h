// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/except.h>
#include <log2/types.h>

namespace Server {
    class Failure final : public Basic::Failure {
    public:
        Failure() = delete;
        Failure(const Failure &) = default;
        Failure(Failure &&) noexcept = default;

        explicit Failure(std::wstring && message, SrcLoc::Point && location = SrcLoc::Point::current())
        : Basic::Failure(std::move(message), Log::c_catWebServer, std::move(location)) {} // NOLINT

        explicit Failure(const std::wstring_view message, SrcLoc::Point && location = SrcLoc::Point::current())
        : Basic::Failure(message, Log::c_catWebServer, std::move(location)) {} // NOLINT

        ~Failure() override = default;

        Failure & operator=(const Failure &) = default;
        Failure & operator=(Failure &&) noexcept = default;
    };
}
