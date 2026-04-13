// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <cstdint>
#include <type_traits>

namespace Ccy {
    constexpr int c_queueDefaultBlockSize { 1024 };
    constexpr int c_queueDefaultBlocksNumberLimit { 16 };
    constexpr bool c_queueDefaultAutoCompletion { true };
    constexpr bool c_queueDefaultSlotHandling { false };
    constexpr bool c_queueDefaultNoExceptConstr { false };
    constexpr bool c_queueDefaultNoExceptAccess { false };
    constexpr unsigned c_queueDefaultAcquireAttempts { 5 };

    enum class QueueSlotState : int8_t { Free, ProdLocked, Ready, ConsLocked };

    class QueueAccessorAutoCompletion {
    public:
        static constexpr bool c_autoComplete { true };

        QueueAccessorAutoCompletion() noexcept = default;
        QueueAccessorAutoCompletion(const QueueAccessorAutoCompletion &) noexcept = default;
        QueueAccessorAutoCompletion(QueueAccessorAutoCompletion &&) noexcept = default;
        virtual ~QueueAccessorAutoCompletion() = default;

        QueueAccessorAutoCompletion & operator=(const QueueAccessorAutoCompletion &) noexcept = default;
        QueueAccessorAutoCompletion & operator=(QueueAccessorAutoCompletion &&) noexcept = default;

        [[maybe_unused]]
        virtual void complete() noexcept final {} // NOLINT
    };

    class QueueAccessorManualCompletion {
    protected:
        bool m_complete { false };

    public:
        static constexpr bool c_autoComplete { false };

        QueueAccessorManualCompletion() noexcept = default;
        QueueAccessorManualCompletion(const QueueAccessorManualCompletion & other) noexcept = default;
        QueueAccessorManualCompletion(QueueAccessorManualCompletion &&) noexcept = default;
        virtual ~QueueAccessorManualCompletion() = default;

        QueueAccessorManualCompletion & operator=(const QueueAccessorManualCompletion &) noexcept = default;
        QueueAccessorManualCompletion & operator=(QueueAccessorManualCompletion &&) noexcept = default;

        [[maybe_unused]]
         virtual void complete() noexcept final {
            m_complete = true;
        }
    };

    template<bool C>
    using QueueAccessorCompletion = std::conditional_t<C, QueueAccessorAutoCompletion, QueueAccessorManualCompletion>;
}
