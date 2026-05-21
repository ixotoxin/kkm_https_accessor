// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include <lib/except.h>
#include <log2/types.h>

namespace Kkm {
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
        : Basic::Failure(message, Log::c_catCashRegister, std::move(location)) {}

        explicit Failure(std::wstring && message, SrcLoc::Point && location = SrcLoc::Point::current())
        // NOLINTNEXTLINE(*-move-const-arg)
        : Basic::Failure(std::move(message), Log::c_catCashRegister, std::move(location)) {}

        explicit Failure(Atol::Fptr & kkm, SrcLoc::Point && location = SrcLoc::Point::current())
        // NOLINTNEXTLINE(*-move-const-arg)
        : Basic::Failure(kkm.errorDescription(), Log::c_catCashRegister, std::move(location)) {
            kkm.resetError();
        }

        explicit Failure(Basic::Wcs::Message && message, SrcLoc::Point && location = SrcLoc::Point::current())
        // NOLINTNEXTLINE(*-move-const-arg)
        : Basic::Failure(std::move(message), Log::c_catCashRegister, std::move(location)) {}

        ~Failure() override = default;

        Failure & operator=(const Failure &) = default;
        Failure & operator=(Failure &&) noexcept = default;
    };
}
