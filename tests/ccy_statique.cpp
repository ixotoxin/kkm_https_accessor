// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <cstdint>
#include <limits>
#include <algorithm>
#include <vector>
#include <thread>
#include <latch>
#include <chrono>
#include <catch2/catch_test_macros.hpp>
#include <ccy/statique.h>
#include "ccy_common.h"

namespace UnitTests {
    namespace Test1 {
        TEST_CASE("Static MPMC queue operated with primitives", "[operating]") {
            using Queue = Ccy::StaticMpmcQueue<Payload, c_stCapacity, true, false, false>;
            Queue queue {};

            REQUIRE(queue.producing());
            REQUIRE(queue.consuming());
            REQUIRE(queue.capacity() == c_stCapacity);
            REQUIRE(queue.freeSlots() == c_stCapacity);
            REQUIRE(queue.empty());

            for (int i { c_stCapacity + c_stOvercapacity }; i; --i) {
                /** Acquire **/
                auto slot = queue.producerSlot();
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot));
                }
                if (slot) {
                    *slot = i;
                }
                /** Default constr (empty slot) **/
                Queue::ProducerAccessor slot2 {};
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                /** Move constr **/
                auto slot3 = std::move(slot);
                REQUIRE_FALSE(static_cast<bool>(slot)); // NOLINT
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot3));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot3));
                }
                if (slot3) {
                    REQUIRE(*slot3 == i);
                }
                /** Move assign **/
                slot2 = std::move(slot3);
                REQUIRE_FALSE(static_cast<bool>(slot3)); // NOLINT
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot2));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot2));
                }
                if (slot2) {
                    REQUIRE(*slot2 == i);
                }
            }

            REQUIRE(queue.producing());
            REQUIRE(queue.consuming());
            REQUIRE(queue.capacity() == c_stCapacity);
            REQUIRE(queue.freeSlots() == 0);
            REQUIRE_FALSE(queue.empty());

            for (int i { c_stCapacity + c_stOvercapacity }; i; --i) {
                /** Acquire **/
                auto slot = queue.consumerSlot();
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot));
                }
                if (slot) {
                    REQUIRE(*slot == i);
                }
                /** Default constr (empty slot) **/
                Queue::ConsumerAccessor slot2 {};
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                /** Move constr **/
                auto slot3 = std::move(slot);
                REQUIRE_FALSE(static_cast<bool>(slot)); // NOLINT
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot3));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot3));
                }
                if (slot3) {
                    REQUIRE(*slot3 == i);
                }
                /** Move assign **/
                slot2 = std::move(slot3);
                REQUIRE_FALSE(static_cast<bool>(slot3)); // NOLINT
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot2));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot2));
                }
                if (slot2) {
                    REQUIRE(*slot2 == i);
                }
            }

            REQUIRE(queue.producing());
            REQUIRE(queue.consuming());
            REQUIRE(queue.capacity() == c_stCapacity);
            REQUIRE(queue.freeSlots() == c_stCapacity);
            REQUIRE(queue.empty());
            queue.shutdown();
            REQUIRE_FALSE(queue.producing());
            REQUIRE(queue.consuming());
            queue.stop();
            REQUIRE_FALSE(queue.producing());
            REQUIRE_FALSE(queue.consuming());

            {
                Queue::ProducerAccessor slot1 {};
                Queue::ProducerAccessor slot2 {};
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot1 = std::move(slot2);
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot2 = std::move(slot1);
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot1 = Queue::ProducerAccessor {};
                slot2 = Queue::ProducerAccessor {};
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot1 = queue.producerSlot();
                slot2 = queue.producerSlot();
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
            }

            {
                Queue::ConsumerAccessor slot1 {};
                Queue::ConsumerAccessor slot2 {};
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot1 = std::move(slot2);
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot2 = std::move(slot1);
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot1 = Queue::ConsumerAccessor {};
                slot2 = Queue::ConsumerAccessor {};
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot1 = queue.consumerSlot();
                slot2 = queue.consumerSlot();
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
            }
        }
    }

    namespace Test2 {
        TEST_CASE("Static MPMC queue operated with structures", "[operating]") {
            using Queue = Ccy::StaticMpmcQueue<Payload, c_stCapacity, true, false, false>;
            Queue queue {};

            REQUIRE(queue.producing());
            REQUIRE(queue.consuming());
            REQUIRE(queue.capacity() == c_stCapacity);
            REQUIRE(queue.freeSlots() == c_stCapacity);
            REQUIRE(queue.empty());

            for (int i { c_stCapacity + c_stOvercapacity }; i; --i) {
                /** Acquire **/
                auto slot = queue.producerSlot();
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot));
                }
                if (slot) {
                    slot->m_str.assign("item");
                    (*slot).m_str.append(std::to_string(i)); // NOLINT
                    slot->m_int = i;
                    (*slot).m_int = i; // NOLINT
                    slot->setBool(i < c_stMagicNumber); // NOLINT
                    (*slot).setBool(slot->getBool()); // NOLINT
                    slot->setBool((*slot).getBool()); // NOLINT
                }
                std::string str { "item" + std::to_string(i) };
                /** Default constr (empty slot) **/
                Queue::ProducerAccessor slot2 {};
                REQUIRE_FALSE(static_cast<bool>(slot2));
                /** Move constr **/
                auto slot3 = std::move(slot);
                REQUIRE_FALSE(static_cast<bool>(slot)); // NOLINT
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot3));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot3));
                }
                if (slot3) {
                    REQUIRE(slot3->m_str == str);
                    REQUIRE((*slot3).m_str == str); // NOLINT
                    REQUIRE(slot3->m_int == i);
                    REQUIRE((*slot3).m_int == i); // NOLINT
                    if (i < c_stMagicNumber) {
                        REQUIRE(slot3->getBool());
                        REQUIRE((*slot3).getBool()); // NOLINT
                    } else {
                        REQUIRE_FALSE(slot3->getBool());
                        REQUIRE_FALSE((*slot3).getBool()); // NOLINT
                    }
                }
                /** Move assign **/
                slot2 = std::move(slot3);
                REQUIRE_FALSE(static_cast<bool>(slot3)); // NOLINT
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot2));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot2));
                }
                if (slot2) {
                    auto & payload = *slot2;
                    REQUIRE(slot2->m_str == str);
                    REQUIRE(payload.m_str == str);
                    REQUIRE(slot2->m_int == i);
                    REQUIRE(payload.m_int == i);
                    if (i < c_stMagicNumber) {
                        REQUIRE(slot2->getBool());
                        REQUIRE(payload.getBool());
                    } else {
                        REQUIRE_FALSE(slot2->getBool());
                        REQUIRE_FALSE(payload.getBool());
                    }
                }
            }

            REQUIRE(queue.producing());
            REQUIRE(queue.consuming());
            REQUIRE(queue.capacity() == c_stCapacity);
            REQUIRE(queue.freeSlots() == 0);
            REQUIRE_FALSE(queue.empty());

            for (int i { c_stCapacity + c_stOvercapacity }; i; --i) {
                std::string str { "item" + std::to_string(i) };
                /** Acquire **/
                auto slot = queue.consumerSlot();
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot));
                }
                if (slot) {
                    REQUIRE(slot->m_str == str);
                    REQUIRE((*slot).m_str == str);
                    REQUIRE(slot->m_int == i);
                    REQUIRE((*slot).m_int == i);
                    if (i < c_stMagicNumber) {
                        REQUIRE(slot->getBool());
                        REQUIRE((*slot).getBool());
                    } else {
                        REQUIRE_FALSE(slot->getBool());
                        REQUIRE_FALSE((*slot).getBool());
                    }
                }
                /** Default constr (empty slot) **/
                Queue::ConsumerAccessor slot2 {};
                REQUIRE_FALSE(static_cast<bool>(slot2));
                /** Move constr **/
                auto slot3 = std::move(slot);
                REQUIRE_FALSE(static_cast<bool>(slot)); // NOLINT
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot3));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot3));
                }
                if (slot3) {
                    REQUIRE(slot3->m_str == str);
                    REQUIRE((*slot3).m_str == str); // NOLINT
                    REQUIRE(slot3->m_int == i);
                    REQUIRE((*slot3).m_int == i); // NOLINT
                    if (i < c_stMagicNumber) {
                        REQUIRE(slot3->getBool());
                        REQUIRE((*slot3).getBool()); // NOLINT
                    } else {
                        REQUIRE_FALSE(slot3->getBool());
                        REQUIRE_FALSE((*slot3).getBool()); // NOLINT
                    }
                }
                /** Move assign **/
                slot2 = std::move(slot3);
                REQUIRE_FALSE(static_cast<bool>(slot3)); // NOLINT
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot2));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot2));
                }
                if (slot2) {
                    auto & payload = *slot2;
                    REQUIRE(slot2->m_str == str);
                    REQUIRE(payload.m_str == str);
                    REQUIRE(slot2->m_int == i);
                    REQUIRE(payload.m_int == i);
                    if (i < c_stMagicNumber) {
                        REQUIRE(slot2->getBool());
                        REQUIRE(payload.getBool());
                    } else {
                        REQUIRE_FALSE(slot2->getBool());
                        REQUIRE_FALSE(payload.getBool());
                    }
                }
            }

            REQUIRE(queue.producing());
            REQUIRE(queue.consuming());
            REQUIRE(queue.capacity() == c_stCapacity);
            REQUIRE(queue.freeSlots() == c_stCapacity);
            REQUIRE(queue.empty());
            queue.shutdown();
            REQUIRE_FALSE(queue.producing());
            REQUIRE(queue.consuming());
            queue.stop();
            REQUIRE_FALSE(queue.producing());
            REQUIRE_FALSE(queue.consuming());

            {
                Queue::ProducerAccessor slot1 {};
                Queue::ProducerAccessor slot2 {};
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot1 = std::move(slot2);
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot2 = std::move(slot1);
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot1 = Queue::ProducerAccessor {};
                slot2 = Queue::ProducerAccessor {};
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot1 = queue.producerSlot();
                slot2 = queue.producerSlot();
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
            }

            {
                Queue::ConsumerAccessor slot1 {};
                Queue::ConsumerAccessor slot2 {};
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot1 = std::move(slot2);
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot2 = std::move(slot1);
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot1 = Queue::ConsumerAccessor {};
                slot2 = Queue::ConsumerAccessor {};
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
                slot1 = queue.consumerSlot();
                slot2 = queue.consumerSlot();
                REQUIRE_FALSE(static_cast<bool>(slot1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(slot2)); // NOLINT
            }
        }
    }

    namespace Test3 {
        TEST_CASE("Order preservation check in a static MPMC queue", "[order]") {
            using Queue = Ccy::StaticMpmcQueue<Payload, c_stCapacity, true, false, false>;
            Queue queue {};

            for (int i { c_stCapacity + c_stOvercapacity }; i; --i) {
                auto slot = queue.producerSlot();
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(slot));
                    *slot = i;
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot));
                }
            }

            for (int i = { c_stCapacity + c_stOvercapacity }; i > c_stCapacity; --i) {
                auto slot = queue.consumerSlot();
                REQUIRE(static_cast<bool>(slot));
                REQUIRE(*slot == i);
            }

            for (int i { c_stCapacity + c_stOvercapacity }; i; --i) {
                auto slot = queue.producerSlot();
                if (i > c_stCapacity) {
                    REQUIRE(static_cast<bool>(slot));
                    *slot = i;
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot));
                }
            }

            for (int i = c_stCapacity; i > c_stOvercapacity; --i) {
                auto slot = queue.consumerSlot();
                REQUIRE(static_cast<bool>(slot));
                REQUIRE(*slot == i);
            }

            REQUIRE_FALSE(queue.empty());

            for (int i = { c_stCapacity + c_stOvercapacity }; i; --i) {
                auto slot = queue.consumerSlot();
                if (i > c_stCapacity) {
                    REQUIRE(static_cast<bool>(slot));
                    REQUIRE(*slot == i);
                } else {
                    REQUIRE_FALSE(static_cast<bool>(slot));
                }
            }

            REQUIRE(queue.producing());
            REQUIRE(queue.consuming());
            REQUIRE(queue.capacity() == c_stCapacity);
            REQUIRE(queue.freeSlots() == c_stCapacity);
            REQUIRE(queue.empty());
            queue.shutdown();
            REQUIRE_FALSE(queue.producing());
            REQUIRE(queue.consuming());
            queue.stop();
            REQUIRE_FALSE(queue.producing());
            REQUIRE_FALSE(queue.consuming());
        }
    }

    namespace Test4 {
        template<bool E>
        using ThrowingCtor = Ccy::StaticMpmcQueue<PayloadThrowingCtor, 100, true, true, E>;

        template<bool E>
        using BaseQueue = Ccy::StaticMpmcQueue<Payload, 100, true, true, E>;

        template<Ccy::AnyStaticMpmcQueue T>
        class ThrowingPrepare : public T {
        protected:
            bool prepare(Payload &) noexcept(T::c_noExceptAccess) override {
                if (!T::c_noExceptAccess) {
                    throwingFunc<PrepareException>();
                }
                return false;
            }

            bool clear(Payload &) noexcept(T::c_noExceptAccess) override {
                return true;
            }
        };

        template<Ccy::AnyStaticMpmcQueue T>
        class ThrowingClean : public T {
        protected:
            bool prepare(Payload &) noexcept(T::c_noExceptAccess) override {
                return true;
            }

            bool clear(Payload &) noexcept(T::c_noExceptAccess) override {
                if (!T::c_noExceptAccess) {
                    throwingFunc<CleanException>();
                }
                return false;
            }
        };

        template<class T>
        void testFunc(int & sequence, int & control) {
            T queue {};
            if (const auto slot = queue.producerSlot(); slot) {
                slot->m_arr[0] = ++sequence;
            } else {
                sequence += 3;
            }
            if (const auto slot = queue.consumerSlot(); slot) {
                control = 1 + slot->m_arr[0];
            } else {
                control = 3 + sequence;
            }
            CHECK(Payload::s_count.load(Ccy::MemOrd::acquire) == queue.capacity());
        }

        template<class T>
        void testFunc2(int & sequence, int & control) {
            auto queue = std::make_shared<T>();
            if (const auto slot = queue->producerSlot(); slot) {
                slot->m_arr[0] = ++sequence;
            } else {
                sequence += 3;
            }
            if (const auto slot = queue->consumerSlot(); slot) {
                control = 1 + slot->m_arr[0];
            } else {
                control = 3 + sequence;
            }
            CHECK(Payload::s_count.load(Ccy::MemOrd::acquire) == queue->capacity());
        }

        TEST_CASE("Static MPMC queue exception correctness check", "[throwing]") {
            int sequence { 42 };
            int control { 42 };

            {
                auto func = testFunc<Ccy::StaticMpmcQueue<Payload, 100, true, false, false>>;
                REQUIRE_NOTHROW(func(sequence, control));
                REQUIRE(sequence == 43);
                REQUIRE(control == 44);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                ++sequence;
            }

            {
                auto func = testFunc<Ccy::StaticMpmcQueue<Payload, 100, true, false, true>>;
                REQUIRE_NOTHROW(func(sequence, control));
                REQUIRE(sequence == 45);
                REQUIRE(control == 46);
            }

#if defined(__clang__)
            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc2<Ccy::StaticMpmcQueue<Payload, c_throwingSize, true, false, false>>;
                REQUIRE_THROWS_AS(func(sequence, control), std::bad_alloc);
                REQUIRE(sequence == 45);
                REQUIRE(control == 46);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc2<Ccy::StaticMpmcQueue<Payload, c_throwingSize, true, false, true>>;
                REQUIRE_THROWS_AS(func(sequence, control), std::bad_alloc);
                REQUIRE(sequence == 45);
                REQUIRE(control == 46);
            }
#endif

            sequence = 48;
            control = 51;

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingCtor<false>>;
                REQUIRE_THROWS_AS(func(sequence, control), CtorException);
                REQUIRE(sequence == 48);
                REQUIRE(control == 51);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingCtor<true>>;
                REQUIRE_THROWS_AS(func(sequence, control), CtorException);
                REQUIRE(sequence == 48);
                REQUIRE(control == 51);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingPrepare<BaseQueue<false>>>;
                REQUIRE_THROWS_AS(func(sequence, control), PrepareException);
                REQUIRE(sequence == 48);
                REQUIRE(control == 51);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingPrepare<BaseQueue<true>>>;
                REQUIRE_NOTHROW(func(sequence, control));
                REQUIRE(sequence == 51);
                REQUIRE(control == 54);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingClean<BaseQueue<false>>>;
                REQUIRE_NOTHROW(func(sequence, control));
                REQUIRE(sequence == 52);
                REQUIRE(control == 53);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingClean<BaseQueue<true>>>;
                REQUIRE_NOTHROW(func(sequence, control));
                REQUIRE(sequence == 53);
                REQUIRE(control == 54);
            }
        }
    }

    namespace Test5 {
        TEST_CASE("Concurrency workloads using a static MPMC queue", "[concurrency]") {
            using Queue = Ccy::StaticMpmcQueue<Payload, c_mtCapacity, true, false, false>;
            Queue queue {};
            std::vector<std::jthread> pool {};
            const unsigned workers { 1 + (std::max(2u, std::thread::hardware_concurrency()) >> 1) };
            std::latch producingLatch { 1 + workers };
            std::latch consumingLatch { 1 + workers };
            std::atomic<Payload> counter { c_mtTestItems };
            std::atomic<Payload> result { 0 };

            for (unsigned i { workers }; i; --i) {
                pool.emplace_back(
                    [& queue, & result, & consumingLatch] {
                        while (queue.consuming() || !queue.empty()) {
                            if (auto slot = queue.consumerSlot(); slot) {
                                result.fetch_add(*slot, Ccy::MemOrd::acq_rel);
                            } else {
                                std::this_thread::yield();
                            }
                        }
                        consumingLatch.count_down();
                    }
                );
            }

            for (unsigned i { workers }; i; --i) {
                pool.emplace_back(
                    [& queue, & counter, & producingLatch] {
                        Payload value { counter.fetch_sub(1, Ccy::MemOrd::acq_rel) };
                        while (queue.producing() && value > 0) {
                            if (auto slot = queue.producerSlot(); slot) {
                                *slot = value;
                                value = counter.fetch_sub(1, Ccy::MemOrd::acq_rel);
                            } else {
                                std::this_thread::yield();
                            }
                        }
                        producingLatch.count_down();
                    }
                );
            }

            {
                auto [cleared, acquired] = queue.clear();
                REQUIRE(cleared == 0);
                REQUIRE(acquired == 0);
            }

            producingLatch.arrive_and_wait();
            queue.gracefulShutdown(c_mtTimeout);
            consumingLatch.arrive_and_wait();

            REQUIRE(queue.capacity() == c_mtCapacity);
            REQUIRE(queue.freeSlots() == c_mtCapacity);
            REQUIRE(queue.empty());
            REQUIRE(result.load() == c_mtExpected);
            REQUIRE_FALSE(queue.producing());
            REQUIRE_FALSE(queue.consuming());

            {
                auto [cleared, acquired] = queue.clear();
                REQUIRE(cleared == 0);
                REQUIRE(acquired == 0);
            }
        }
    }

    namespace Test6 {
        static_assert(c_mtCapacity > c_mtMagicNumber1);
        static_assert(c_mtTestItems > c_mtMagicNumber2);

        TEST_CASE("Concurrency workloads using a static MPMC queue without auto completion", "[concurrency]") {
            using Queue = Ccy::StaticMpmcQueue<Payload, c_mtCapacity, false, false, false>;
            Queue queue {};
            std::vector<std::jthread> pool {};
            const unsigned workers { 1 + (std::max(2u, std::thread::hardware_concurrency()) >> 1) };
            std::latch producingLatch { 1 + workers };
            std::latch consumingLatch { 1 + workers };
            std::atomic<Payload> counter { c_mtTestItems };
            std::atomic<Payload> result { 0 };

            for (unsigned i { workers }; i; --i) {
                pool.emplace_back(
                    [& queue, & result, & consumingLatch] {
                        while (queue.consuming() || !queue.empty()) {
                            if (auto slot = queue.consumerSlot(); slot) {
                                const auto value = *slot;
                                if (value > c_mtMagicNumber1) {
                                    result.fetch_add(*slot, Ccy::MemOrd::acq_rel);
                                    slot.complete();
                                }
                            } else {
                                std::this_thread::yield();
                            }
                        }
                        consumingLatch.count_down();
                    }
                );
            }

            for (unsigned i { workers }; i; --i) {
                pool.emplace_back(
                    [& queue, & counter, & producingLatch] {
                        Payload value { counter.fetch_sub(1, Ccy::MemOrd::acq_rel) };
                        while (queue.producing() && value > 0) {
                            if (auto slot = queue.producerSlot(); slot) {
                                if (value <= c_mtMagicNumber2) {
                                    *slot = value;
                                    slot.complete();
                                }
                                value = counter.fetch_sub(1, Ccy::MemOrd::acq_rel);
                            } else {
                                std::this_thread::yield();
                            }
                        }
                        producingLatch.count_down();
                    }
                );
            }

            {
                auto [cleared, acquired] = queue.clear();
                REQUIRE(cleared == 0);
                REQUIRE(acquired == 0);
            }

            producingLatch.arrive_and_wait();
            queue.shutdown();

            REQUIRE(queue.capacity() == c_mtCapacity);
            REQUIRE(queue.freeSlots() <= queue.capacity() - c_mtMagicNumber1);
            REQUIRE_FALSE(queue.empty());
            REQUIRE_FALSE(queue.producing());
            REQUIRE(queue.consuming());

            {
                bool complete { true };
                auto deadline = std::chrono::steady_clock::now() + c_mtTimeout;
                while (!queue.empty()) {
                    if (auto slot = queue.consumerSlot(); slot) {
                        if (*slot <= c_mtMagicNumber1) {
                            slot.complete();
                        }
                    }
                    if (std::chrono::steady_clock::now() > deadline) {
                        complete = false;
                        break;
                    }
                }
                REQUIRE(complete);
            }

            queue.stop();
            consumingLatch.arrive_and_wait();

            REQUIRE(queue.capacity() == c_mtCapacity);
            REQUIRE(queue.freeSlots() == c_mtCapacity);
            REQUIRE(queue.empty());
            REQUIRE(result.load() == c_mtExpected);
            REQUIRE_FALSE(queue.producing());
            REQUIRE_FALSE(queue.consuming());

            {
                auto [cleared, acquired] = queue.clear();
                REQUIRE(cleared == 0);
                REQUIRE(acquired == 0);
            }
        }
    }
}
