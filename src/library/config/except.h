// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/except.h>
#include <log2/types.h>

namespace Config {
    using Basic::Wcs::Cat;
    using Basic::Wcs::Fmt;

    class Failure final : public Basic::Failure {
    public:
        Failure() = delete;
        Failure(const Failure &) = default;
        Failure(Failure &&) noexcept = default;

        explicit Failure(const std::wstring_view message, SrcLoc::Point && location = SrcLoc::Point::current())
        : Basic::Failure(message, Log::c_catConfiguration, std::move(location)) {} // NOLINT

        explicit Failure(std::wstring && message, SrcLoc::Point && location = SrcLoc::Point::current())
        : Basic::Failure(std::move(message), Log::c_catConfiguration, std::move(location)) {} // NOLINT

        explicit Failure(Basic::Wcs::Message && message, SrcLoc::Point && location = SrcLoc::Point::current())
        : Basic::Failure(std::move(message), Log::c_catConfiguration, std::move(location)) {} // NOLINT

        ~Failure() override = default;

        Failure & operator=(const Failure &) = default;
        Failure & operator=(Failure &&) noexcept = default;
    };
}
