// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "strings.h"
#include "srcloc.h"
#include "msgbuild.h"

namespace Basic {
    class Failure {
    protected:
        std::wstring m_message {};
        SrcLoc::Point m_location;
        unsigned short m_category;

    public:
        Failure() = delete;
        Failure(const Failure &) = default;
        Failure(Failure &&) = default;

        [[maybe_unused]]
        explicit Failure(
            std::wstring && message,
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : m_message(std::move(message)), m_location(std::move(location)), m_category(category) {} // NOLINT

        [[maybe_unused]]
        explicit Failure(
            const std::wstring_view message,
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : m_message(message.data(), message.size()), m_location(std::move(location)), m_category(category) {} // NOLINT

        [[maybe_unused]]
        explicit Failure(
            Wcs::Message && message,
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : m_message(std::move(message.m_message)), m_location(std::move(location)), m_category(category) {} // NOLINT

        virtual ~Failure() = default;

        Failure & operator=(const Failure &) = default;
        Failure & operator=(Failure &&) noexcept = default;

        [[maybe_unused]]
        bool category(const unsigned short clarifyingCategory) noexcept {
            if (m_category == 0) {
                m_category = clarifyingCategory;
                return true;
            }
            return false;
        }

        [[nodiscard, maybe_unused]]
        unsigned short category() const noexcept {
            return m_category;
        }

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
        virtual void explain(std::wstring & receiver) const noexcept {
            receiver.append(m_message);
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
        : Failure(std::move(e)), m_variable(variable) {}

        [[maybe_unused]]
        explicit DataError(
            std::wstring && message,
            const std::wstring_view variable = {},
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : Failure(std::move(message), category, std::move(location)), m_variable(variable) {} // NOLINT

        [[maybe_unused]]
        explicit DataError(
            const std::wstring_view message,
            const std::wstring_view variable = {},
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : Failure(message, category, std::move(location)), m_variable(variable) {} // NOLINT

        [[maybe_unused]]
        explicit DataError(
            Wcs::Message && message,
            const std::wstring_view variable = {},
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : Failure(std::move(message.m_message), category, std::move(location)), m_variable(variable) {} // NOLINT

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
            if (m_variable.empty()) {
                return m_message;
            }
            std::wstring result {};
            result.reserve(Wcs::c_dataError.size() + m_message.size() + m_variable.size());
            std::vformat_to(
                std::back_inserter(result),
                Wcs::c_dataError,
                std::make_wformat_args(m_message, m_variable)
            );
            return result;
        }

        [[maybe_unused]]
        void explain(std::wstring & output) const noexcept override {
            if (m_variable.empty()) {
                output.append(m_message);
            } else {
                const size_t size { output.size() + Wcs::c_dataError.size() + m_message.size() + m_variable.size() };
                if (output.capacity() < size) {
                    output.reserve(size);
                }
                std::vformat_to(
                    std::back_inserter(output),
                    Wcs::c_dataError,
                    std::make_wformat_args(m_message, m_variable)
                );
            }
        }
    };
}
