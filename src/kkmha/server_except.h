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
        Failure(const Failure &) = default;
        Failure(Failure &&) noexcept = default;

        explicit Failure(const std::wstring_view message, SrcLoc::Point && location = SrcLoc::Point::current())
        // NOLINTNEXTLINE(*-move-const-arg)
        : Basic::Failure(message, Log::c_catWebServer, std::move(location)) {}

        explicit Failure(std::wstring && message, SrcLoc::Point && location = SrcLoc::Point::current())
        // NOLINTNEXTLINE(*-move-const-arg)
        : Basic::Failure(std::move(message), Log::c_catWebServer, std::move(location)) {}

        explicit Failure(Basic::Wcs::Message && message, SrcLoc::Point && location = SrcLoc::Point::current())
        // NOLINTNEXTLINE(*-move-const-arg)
        : Basic::Failure(std::move(message), Log::c_catWebServer, std::move(location)) {}

        ~Failure() override = default;

        Failure & operator=(const Failure &) = default;
        Failure & operator=(Failure &&) noexcept = default;
    };
}
