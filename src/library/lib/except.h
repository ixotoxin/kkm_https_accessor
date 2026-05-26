// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "srcloc.h"
#include "msgbuild.h"

namespace Basic {
    class Failure {
    protected:
        std::wstring m_message {};
        SrcLoc::Point m_location {};
        unsigned short m_category {};

    public:
        Failure() = delete;
        [[maybe_unused]] Failure(const Failure &);
        [[maybe_unused]] Failure(Failure &&) noexcept;
        [[maybe_unused]] explicit Failure(std::wstring &&, unsigned short = 0, SrcLoc::Point = SrcLoc::Point::current());
        [[maybe_unused]] explicit Failure(std::wstring_view, unsigned short = 0, SrcLoc::Point = SrcLoc::Point::current());
        [[maybe_unused]] explicit Failure(Wcs::Message &&, unsigned short = 0, SrcLoc::Point = SrcLoc::Point::current());
        virtual ~Failure();

        Failure & operator=(const Failure &) = delete;
        Failure & operator=(Failure &&) = delete;

        [[maybe_unused]] bool category(unsigned short clarifyingCategory) noexcept;
        [[nodiscard, maybe_unused]] unsigned short category() const noexcept;
        [[nodiscard, maybe_unused]] const std::wstring & what() const noexcept;
        [[nodiscard, maybe_unused]] const SrcLoc::Point & where() const noexcept;
        [[nodiscard, maybe_unused]] virtual std::wstring explain() const noexcept;
        [[maybe_unused]] virtual void explain(std::wstring & receiver) const noexcept;
    };

    class DataError : public Failure {
    protected:
        std::wstring m_variable {};
        bool m_isPointer {};

    public:
        DataError() = delete;
        [[maybe_unused]] DataError(const DataError &);
        [[maybe_unused]] DataError(DataError &&) noexcept;
        [[maybe_unused]] DataError(Failure && e, std::wstring &&, bool = false);
        [[maybe_unused]] explicit DataError(Failure &&, std::wstring_view = {}, bool = false);

        [[maybe_unused]]
        explicit DataError(
            std::wstring &&,
            std::wstring &&,
            bool = false,
            unsigned short = 0,
            SrcLoc::Point = SrcLoc::Point::current()
        );

        [[maybe_unused]]
        explicit DataError(
            std::wstring &&,
            std::wstring_view = {},
            bool = false,
            unsigned short = 0,
            SrcLoc::Point = SrcLoc::Point::current()
        );

        [[maybe_unused]]
        explicit DataError(
            std::wstring_view,
            std::wstring &&,
            bool = false,
            unsigned short = 0,
            SrcLoc::Point = SrcLoc::Point::current()
        );

        [[maybe_unused]]
        explicit DataError(
            std::wstring_view,
            std::wstring_view = {},
            bool = false,
            unsigned short = 0,
            SrcLoc::Point = SrcLoc::Point::current()
        );

        [[maybe_unused]]
        explicit DataError(
            Wcs::Message &&,
            std::wstring &&,
            bool = false,
            unsigned short = 0,
            SrcLoc::Point = SrcLoc::Point::current()
        );

        [[maybe_unused]]
        explicit DataError(
            Wcs::Message &&,
            std::wstring_view = {},
            bool = false,
            unsigned short = 0,
            SrcLoc::Point = SrcLoc::Point::current()
        );

        ~DataError() override;

        DataError & operator=(const DataError &) = delete;
        DataError & operator=(DataError &&) = delete;

        [[maybe_unused]] void variable(std::wstring &&, bool = false, bool override = false) noexcept;
        [[maybe_unused]] void variable(std::wstring_view, bool = false, bool = false) noexcept;
        [[nodiscard, maybe_unused]] const std::wstring & variable() const noexcept;
        [[nodiscard, maybe_unused]] std::wstring explain() const noexcept override;
        [[maybe_unused]] void explain(std::wstring & output) const noexcept override;
    };
}
