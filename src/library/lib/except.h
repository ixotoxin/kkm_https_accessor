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
        // NOLINTNEXTLINE(*-move-const-arg)
        ) : m_message { std::move(message) }, m_location { std::move(location) },
            m_category { category } {}

        [[maybe_unused]]
        explicit Failure(
            const std::wstring_view message,
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        // NOLINTNEXTLINE(*-move-const-arg)
        ) : m_message { message.data() , message.size() }, m_location { std::move(location) },
            m_category { category } {}

        [[maybe_unused]]
        explicit Failure(
            Wcs::Message && message,
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        // NOLINTNEXTLINE(*-move-const-arg)
        ) : m_message { std::move(message.m_message) }, m_location { std::move(location) },
            m_category { category } {}

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
        bool m_isPointer;

    public:
        DataError() = delete;
        DataError(const DataError &) = default;
        DataError(DataError &&) = default;

        [[maybe_unused]]
        explicit DataError(Failure && e, const std::wstring_view variable = {}, const bool isPointer = false)
        : Failure(std::move(e)), m_variable { variable }, m_isPointer { isPointer } {}

        [[maybe_unused]]
        DataError(Failure && e, std::wstring && variable, const bool isPointer = false)
        : Failure(std::move(e)), m_variable { std::move(variable) }, m_isPointer { isPointer } {}

        [[maybe_unused]]
        explicit DataError(
            std::wstring && message,
            const std::wstring_view variable = {},
            const bool isPointer = false,
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        // NOLINTNEXTLINE(*-move-const-arg)
        ) : Failure(std::move(message), category, std::move(location)),
            m_variable { variable }, m_isPointer { isPointer } {}

        [[maybe_unused]]
        explicit DataError(
            std::wstring && message,
            std::wstring && variable,
            const bool isPointer = false,
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        // NOLINTNEXTLINE(*-move-const-arg)
        ) : Failure(std::move(message), category, std::move(location)),
            m_variable { std::move(variable) }, m_isPointer { isPointer } {}

        [[maybe_unused]]
        explicit DataError(
            const std::wstring_view message,
            const std::wstring_view variable = {},
            const bool isPointer = false,
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        // NOLINTNEXTLINE(*-move-const-arg)
        ) : Failure(message, category, std::move(location)),
            m_variable { variable }, m_isPointer { isPointer } {}

        [[maybe_unused]]
        explicit DataError(
            const std::wstring_view message,
            std::wstring && variable,
            const bool isPointer = false,
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        // NOLINTNEXTLINE(*-move-const-arg)
        ) : Failure(message, category, std::move(location)),
            m_variable { std::move(variable) }, m_isPointer { isPointer } {}

        [[maybe_unused]]
        explicit DataError(
            Wcs::Message && message,
            const std::wstring_view variable = {},
            const bool isPointer = false,
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        // NOLINTNEXTLINE(*-move-const-arg)
        ) : Failure(std::move(message.m_message), category, std::move(location)),
            m_variable { variable }, m_isPointer { isPointer } {}

        [[maybe_unused]]
        explicit DataError(
            Wcs::Message && message,
            std::wstring && variable,
            const bool isPointer = false,
            const unsigned short category = 0,
            SrcLoc::Point && location = SrcLoc::Point::current()
        // NOLINTNEXTLINE(*-move-const-arg)
        ) : Failure(std::move(message.m_message), category, std::move(location)),
            m_variable { std::move(variable) }, m_isPointer { isPointer } {}

        ~DataError() override = default;

        DataError & operator=(const DataError &) = default;
        DataError & operator=(DataError &&) noexcept = default;

        [[maybe_unused]]
        void variable(
            const std::wstring_view variable,
            const bool isPointer = false,
            const bool override = false
        ) noexcept {
            if (override || (m_variable.empty() && !variable.empty())) {
                m_variable.assign(variable);
                m_isPointer = isPointer;
            }
        }

        [[maybe_unused]]
        void variable(
            std::wstring && variable,
            const bool isPointer = false,
            const bool override = false
        ) noexcept {
            if (override || (m_variable.empty() && !variable.empty())) {
                m_variable.assign(std::move(variable));
                m_isPointer = isPointer;
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
            const std::wstring_view typeName { m_isPointer ? Wcs::c_invalidPointer : Wcs::c_invalidVariable };
            std::wstring result {};
            result.reserve(typeName.size() + m_message.size() + m_variable.size() + 6);
            result.assign(m_message);
            result.append(L" (");
            result.assign(typeName);
            result.append(L" '");
            result.assign(m_variable);
            result.append(L"')");
            return result;
        }

        [[maybe_unused]]
        void explain(std::wstring & output) const noexcept override {
            if (m_variable.empty()) {
                output.append(m_message);
            } else {
                const std::wstring_view typeName { m_isPointer ? Wcs::c_invalidPointer : Wcs::c_invalidVariable };
                const size_t size { output.size() + typeName.size() + m_message.size() + m_variable.size() + 6 };
                if (output.capacity() < size) {
                    output.reserve(size);
                }
                output.assign(m_message);
                output.append(L" (");
                output.assign(typeName);
                output.append(L" '");
                output.assign(m_variable);
                output.append(L"')");
            }
        }
    };
}
