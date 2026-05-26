// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/msgbuild.h>
#include <lib/except.h>
#include <log2/types.h>

namespace Server {
    using Basic::Wcs::Cat;
    using Basic::Wcs::Fmt;
    using Basic::DataError;

    class Failure final : public Basic::Failure {
    public:
        Failure() = delete;
        [[maybe_unused]] Failure(const Failure &) = default;
        [[maybe_unused]] Failure(Failure &&) noexcept = default;

        [[maybe_unused]]
        // NOLINTNEXTLINE(performance-unnecessary-value-param)
        explicit Failure(const std::wstring_view message, SrcLoc::Point location = SrcLoc::Point::current())
        : Basic::Failure(message, Log::c_catWebServer, location) {}

        [[maybe_unused]]
        // NOLINTNEXTLINE(performance-unnecessary-value-param)
        explicit Failure(std::wstring && message, SrcLoc::Point location = SrcLoc::Point::current())
        : Basic::Failure(std::move(message), Log::c_catWebServer, location) {}

        [[maybe_unused]]
        // NOLINTNEXTLINE(performance-unnecessary-value-param)
        explicit Failure(Basic::Wcs::Message && message, SrcLoc::Point location = SrcLoc::Point::current())
        : Basic::Failure(std::move(message), Log::c_catWebServer, location) {}

        ~Failure() override = default;

        Failure & operator=(const Failure &) = delete;
        Failure & operator=(Failure &&) = delete;
    };
}
