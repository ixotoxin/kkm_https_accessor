// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "strings.h"
#include "wconv.h"
#include "srcloc.h"
#include <utility>
#include <system_error>
#include <string>
#include <format>

namespace Basic {
    class Failure {
    protected:
        std::wstring m_message {};
        SrcLoc::Point m_location;

    public:
        Failure() = delete;
        Failure(const Failure &) = default;
        Failure(Failure &&) = default;

        [[maybe_unused]]
        explicit Failure(
            const std::wstring_view message,
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : m_message(message.begin(), message.end()), m_location(std::forward<SrcLoc::Point>(location)) {}

        [[maybe_unused]]
        explicit Failure(
            const std::string_view message,
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : m_message(Text::convert(message)), m_location(std::forward<SrcLoc::Point>(location)) {}

        [[maybe_unused]]
        explicit Failure(
            const std::exception & e,
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : m_message(Text::convert(e.what())), m_location(std::forward<SrcLoc::Point>(location)) {}

        [[maybe_unused]]
        explicit Failure(
            const std::error_code & e,
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : m_message(Text::convert(e.message())), m_location(std::forward<SrcLoc::Point>(location)) {}

        virtual ~Failure() = default;

        Failure & operator=(const Failure &) = default;
        Failure & operator=(Failure &&) noexcept = default;

        [[nodiscard, maybe_unused]]
        const std::wstring & what() const noexcept {
            return m_message;
        }

        [[nodiscard, maybe_unused]]
        const SrcLoc::Point & where() const noexcept {
            return m_location;
        }

        [[nodiscard, maybe_unused]]
        virtual std::wstring explain() const noexcept {
            return m_message;
        }

        [[maybe_unused]]
        virtual void appendExplanation(std::wstring & result) const noexcept {
            result.append(m_message);
        }

        [[nodiscard, maybe_unused]]
        virtual std::wstring explain(const bool appendLocation) const noexcept {
            std::wstring result { m_message };
            if (appendLocation) {
                result += m_location;
            }
            return result;
        }

        [[maybe_unused]]
        virtual void appendExplanation(std::wstring & result, const bool appendLocation) const noexcept {
            result.append(m_message);
            if (appendLocation) {
                result += m_location;
            }
        }
    };

    class DataError : public Failure {
    protected:
        std::wstring m_variable;

    public:
        DataError() = delete;
        DataError(const DataError &) = default;
        DataError(DataError &&) = default;

        [[maybe_unused]]
        explicit DataError(Failure && e, const std::wstring_view variable = {})
        : Failure(std::forward<Failure>(e)), m_variable(variable) {}

        [[maybe_unused]]
        explicit DataError(
            const std::wstring_view message,
            const std::wstring_view variable = {},
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : Failure(message, std::forward<SrcLoc::Point>(location)), m_variable(variable) {}

        [[maybe_unused]]
        explicit DataError(
            const std::string_view message,
            const std::wstring_view variable = {},
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : Failure(message, std::forward<SrcLoc::Point>(location)), m_variable(variable) {}

        [[maybe_unused]]
        explicit DataError(
            const std::exception & e,
            const std::wstring_view variable = {},
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : Failure(e, std::forward<SrcLoc::Point>(location)), m_variable(variable) {}

        [[maybe_unused]]
        explicit DataError(
            const std::error_code & e,
            const std::wstring_view variable = {},
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : Failure(e, std::forward<SrcLoc::Point>(location)), m_variable(variable) {}

        ~DataError() override = default;

        DataError & operator=(const DataError &) = default;
        DataError & operator=(DataError &&) noexcept = default;

        [[maybe_unused]]
        void variable(const std::wstring_view variable, const bool override = false) noexcept {
            if (override || (m_variable.empty() && !variable.empty())) {
                m_variable.assign(variable);
            }
        }

        [[nodiscard, maybe_unused]]
        const std::wstring & variable() const noexcept {
            return m_variable;
        }

        [[nodiscard, maybe_unused]]
        std::wstring explain() const noexcept override {
            return m_variable.empty()
               ? m_message
               : LIB_WFMT(Wcs::c_dataError, m_message, m_variable);
        }

        [[maybe_unused]]
        void appendExplanation(std::wstring & result) const noexcept override {
            if (m_variable.empty()) {
                result.append(m_message);
            } else {
                LIB_WFMT2(result, Wcs::c_dataError, m_message, m_variable);
            }
        }

        [[nodiscard, maybe_unused]]
        std::wstring explain(const bool appendLocation) const noexcept override {
            std::wstring message {
                m_variable.empty()
                ? m_message
                : LIB_WFMT(Wcs::c_dataError, m_message, m_variable)
            };
            if (appendLocation) {
                message += m_location;
            }
            return message;
        }

        [[maybe_unused]]
        void appendExplanation(std::wstring & result, const bool appendLocation) const noexcept override {
            if (m_variable.empty()) {
                result.append(m_message);
            } else {
                LIB_WFMT2(result, Wcs::c_dataError, m_message, m_variable);
            }
            if (appendLocation) {
                result += m_location;
            }
        }
    };
}
