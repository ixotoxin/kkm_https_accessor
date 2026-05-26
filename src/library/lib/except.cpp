// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "except.h"

namespace Basic {
    [[maybe_unused]] Failure::Failure(const Failure &) = default;
    [[maybe_unused]] Failure::Failure(Failure &&) noexcept = default;

    [[maybe_unused]]
    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    Failure::Failure(std::wstring && message, const unsigned short category, SrcLoc::Point location)
    : m_message { std::move(message) }, m_location { location }, m_category { category } {}

    [[maybe_unused]]
    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    Failure::Failure(const std::wstring_view message, const unsigned short category, SrcLoc::Point location)
    : m_message { message.data() , message.size() }, m_location { location }, m_category { category } {}

    [[maybe_unused]]
    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    Failure::Failure(Wcs::Message && message, const unsigned short category, SrcLoc::Point location)
    : m_message { std::move(message.m_message) }, m_location { location }, m_category { category } {}

    Failure::~Failure() = default;

    [[maybe_unused]]
    bool Failure::category(const unsigned short clarifyingCategory) noexcept {
        if (m_category == 0) {
            m_category = clarifyingCategory;
            return true;
        }
        return false;
    }

    [[nodiscard, maybe_unused]]
    unsigned short Failure::category() const noexcept {
        return m_category;
    }

    [[nodiscard, maybe_unused]]
    const std::wstring & Failure::what() const noexcept {
        return m_message;
    }

    [[nodiscard, maybe_unused]]
    const SrcLoc::Point & Failure::where() const noexcept {
        return m_location;
    }

    [[nodiscard, maybe_unused]]
    std::wstring Failure::explain() const noexcept {
        return m_message;
    }

    [[maybe_unused]]
    void Failure::explain(std::wstring & receiver) const noexcept {
        receiver.append(m_message);
    }

    [[maybe_unused]] DataError::DataError(const DataError &) = default;
    [[maybe_unused]] DataError::DataError(DataError &&) noexcept = default;

    [[maybe_unused]]
    DataError::DataError(Failure && e, std::wstring && variable, const bool isPointer)
    : Failure(std::move(e)), m_variable { std::move(variable) }, m_isPointer { isPointer } {}

    [[maybe_unused]]
    DataError::DataError(Failure && e, const std::wstring_view variable, const bool isPointer)
    : Failure(std::move(e)), m_variable { variable.data(), variable.size() }, m_isPointer { isPointer } {}

    [[maybe_unused]]
    DataError::DataError(
        std::wstring && message,
        std::wstring && variable,
        const bool isPointer,
        const unsigned short category,
        SrcLoc::Point location // NOLINT(performance-unnecessary-value-param)
    ) : Failure(std::move(message), category, location),
        m_variable { std::move(variable) }, m_isPointer { isPointer } {}

    [[maybe_unused]]
    DataError::DataError(
        std::wstring && message,
        const std::wstring_view variable,
        const bool isPointer,
        const unsigned short category,
        SrcLoc::Point location // NOLINT(performance-unnecessary-value-param)
    ) : Failure(std::move(message), category, location),
        m_variable { variable.data(), variable.size() }, m_isPointer { isPointer } {}

    [[maybe_unused]]
    DataError::DataError(
        const std::wstring_view message,
        std::wstring && variable,
        const bool isPointer,
        const unsigned short category,
        SrcLoc::Point location // NOLINT(performance-unnecessary-value-param)
    ) : Failure(message, category, location),
        m_variable { std::move(variable) }, m_isPointer { isPointer } {}

    [[maybe_unused]]
    DataError::DataError(
        const std::wstring_view message,
        const std::wstring_view variable,
        const bool isPointer,
        const unsigned short category,
        SrcLoc::Point location // NOLINT(performance-unnecessary-value-param)
    ) : Failure(message, category, location),
        m_variable { variable.data(), variable.size() }, m_isPointer { isPointer } {}

    [[maybe_unused]]
    DataError::DataError(
        Wcs::Message && message,
        std::wstring && variable,
        const bool isPointer,
        const unsigned short category,
        SrcLoc::Point location // NOLINT(performance-unnecessary-value-param)
    ) : Failure(std::move(message.m_message), category, location),
        m_variable { std::move(variable) }, m_isPointer { isPointer } {}

    [[maybe_unused]]
    DataError::DataError(
        Wcs::Message && message,
        const std::wstring_view variable,
        const bool isPointer,
        const unsigned short category,
        SrcLoc::Point location // NOLINT(performance-unnecessary-value-param)
    ) : Failure(std::move(message.m_message), category, location),
        m_variable { variable.data(), variable.size() }, m_isPointer { isPointer } {}

    DataError::~DataError() = default;

    [[maybe_unused]]
    void DataError::variable(
        std::wstring && variable,
        const bool isPointer,
        const bool override
    ) noexcept {
        if (override || (m_variable.empty() && !variable.empty())) {
            m_variable.assign(std::move(variable));
            m_isPointer = isPointer;
        }
    }

    [[maybe_unused]]
    void DataError::variable(
        const std::wstring_view variable,
        const bool isPointer,
        const bool override
    ) noexcept {
        if (override || (m_variable.empty() && !variable.empty())) {
            m_variable.assign(variable.data(), variable.size());
            m_isPointer = isPointer;
        }
    }

    [[nodiscard, maybe_unused]]
    const std::wstring & DataError::variable() const noexcept {
        return m_variable;
    }

    [[nodiscard, maybe_unused]]
    std::wstring DataError::explain() const noexcept {
        if (m_variable.empty()) {
            return m_message;
        }
        const std::wstring_view typeName { m_isPointer ? Wcs::c_invalidPointer : Wcs::c_invalidVariable };
        std::wstring result {};
        result.reserve(typeName.size() + m_message.size() + m_variable.size() + 6);
        result.assign(m_message);
        result.append(L" (");
        result.append(typeName);
        result.append(L" '");
        result.append(m_variable);
        result.append(L"')");
        return result;
    }

    [[maybe_unused]]
    void DataError::explain(std::wstring & output) const noexcept {
        if (m_variable.empty()) {
            output.append(m_message);
        } else {
            const std::wstring_view typeName { m_isPointer ? Wcs::c_invalidPointer : Wcs::c_invalidVariable };
            const size_t size { output.size() + typeName.size() + m_message.size() + m_variable.size() + 6 }; // NOLINT
            if (output.capacity() < size) {
                output.reserve(size);
            }
            output.append(m_message);
            output.append(L" (");
            output.append(typeName);
            output.append(L" '");
            output.append(m_variable);
            output.append(L"')");
        }
    }
}
