// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <concepts>
#include <latch>
#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <ccy/dynrespool.h>
#include "ccy_common.h"

namespace UnitTests {
    namespace Test1 {
        TEST_CASE("Dynamic resource pool operated with primitives", "[operating]") {
            using Pool = Ccy::DynamicResourcePool<Payload, c_stBlockSize>;
            auto pool = Ccy::make<Pool>(c_stMaxBlocks);

            REQUIRE(pool->capacity() == c_stBlockSize);
            REQUIRE(pool->freeItems() == c_stBlockSize);
            REQUIRE_FALSE(pool->empty());

            std::vector<Pool::Accessor> lockedItems {};

            for (int i { c_stCapacity + c_stOvercapacity }; i; --i) {
                /** Acquire **/
                auto item = pool->acquire();
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(item));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(item));
                }
                if (item) {
                    *item = i;
                }
                /** Default constr (empty item) **/
                Pool::Accessor item2 {};
                REQUIRE_FALSE(static_cast<bool>(item2)); // NOLINT
                /** Move constr **/
                auto item3 = std::move(item);
                REQUIRE_FALSE(static_cast<bool>(item)); // NOLINT
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(item3));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(item3));
                }
                if (item3) {
                    REQUIRE(*item3 == i);
                }
                /** Move assign **/
                item2 = std::move(item3);
                REQUIRE_FALSE(static_cast<bool>(item3)); // NOLINT
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(item2));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(item2));
                }
                if (item2) {
                    REQUIRE(*item2 == i);
                    lockedItems.push_back(std::move(item2));
                }
            }

            REQUIRE(pool->capacity() == c_stCapacity);
            REQUIRE(pool->freeItems() == 0);
            REQUIRE(pool->empty());

            lockedItems.clear();

            REQUIRE(pool->capacity() == c_stCapacity);
            REQUIRE(pool->freeItems() == c_stCapacity);
            REQUIRE_FALSE(pool->empty());

            {
                Pool::Accessor item1 {};
                Pool::Accessor item2 {};
                REQUIRE_FALSE(static_cast<bool>(item1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(item2)); // NOLINT
                item1 = std::move(item2);
                REQUIRE_FALSE(static_cast<bool>(item1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(item2)); // NOLINT
                item2 = std::move(item1);
                REQUIRE_FALSE(static_cast<bool>(item1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(item2)); // NOLINT
                item1 = Pool::Accessor {};
                item2 = Pool::Accessor {};
                REQUIRE_FALSE(static_cast<bool>(item1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(item2)); // NOLINT
                item1 = pool->acquire();
                REQUIRE(static_cast<bool>(item1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(item2)); // NOLINT
                item2 = std::move(item1);
                REQUIRE_FALSE(static_cast<bool>(item1)); // NOLINT
                REQUIRE(static_cast<bool>(item2)); // NOLINT
            }
        }
    }

    namespace Test2 {
        TEST_CASE("Dynamic resource pool operated with structures", "[operating]") {
            using Pool = Ccy::DynamicResourcePool<Payload, c_stBlockSize>;
            auto pool = Ccy::make<Pool>(c_stMaxBlocks);

            REQUIRE(pool->capacity() == c_stBlockSize);
            REQUIRE(pool->freeItems() == c_stBlockSize);
            REQUIRE_FALSE(pool->empty());

            std::vector<Pool::Accessor> lockedItems {};

            for (int i { c_stCapacity + c_stOvercapacity }; i; --i) {
                /** Acquire **/
                auto item = pool->acquire();
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(item));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(item));
                }
                if (item) {
                    item->m_str.assign("item");
                    (*item).m_str.append(std::to_string(i)); // NOLINT
                    item->m_int = i;
                    (*item).m_int = i; // NOLINT
                    item->setBool(i < c_stMagicNumber); // NOLINT
                    (*item).setBool(item->getBool()); // NOLINT
                    item->setBool((*item).getBool()); // NOLINT
                }
                std::string str { "item" + std::to_string(i) };
                /** Default constr (empty item) **/
                Pool::Accessor item2 {};
                REQUIRE_FALSE(static_cast<bool>(item2));
                /** Move constr **/
                auto item3 = std::move(item);
                REQUIRE_FALSE(static_cast<bool>(item)); // NOLINT
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(item3));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(item3));
                }
                if (item3) {
                    REQUIRE(item3->m_str == str);
                    REQUIRE((*item3).m_str == str); // NOLINT
                    REQUIRE(item3->m_int == i);
                    REQUIRE((*item3).m_int == i); // NOLINT
                    if (i < c_stMagicNumber) {
                        REQUIRE(item3->getBool());
                        REQUIRE((*item3).getBool()); // NOLINT
                    } else {
                        REQUIRE_FALSE(item3->getBool());
                        REQUIRE_FALSE((*item3).getBool()); // NOLINT
                    }
                }
                /** Move assign **/
                item2 = std::move(item3);
                REQUIRE_FALSE(static_cast<bool>(item3)); // NOLINT
                if (i > c_stOvercapacity) {
                    REQUIRE(static_cast<bool>(item2));
                } else {
                    REQUIRE_FALSE(static_cast<bool>(item2));
                }
                if (item2) {
                    auto & payload = *item2;
                    REQUIRE(item2->m_str == str);
                    REQUIRE(payload.m_str == str);
                    REQUIRE(item2->m_int == i);
                    REQUIRE(payload.m_int == i);
                    if (i < c_stMagicNumber) {
                        REQUIRE(item2->getBool());
                        REQUIRE(payload.getBool());
                    } else {
                        REQUIRE_FALSE(item2->getBool());
                        REQUIRE_FALSE(payload.getBool());
                    }
                    lockedItems.push_back(std::move(item2));
                }
            }

            REQUIRE(pool->capacity() == c_stCapacity);
            REQUIRE(pool->freeItems() == 0);
            REQUIRE(pool->empty());

            lockedItems.clear();

            REQUIRE(pool->capacity() == c_stCapacity);
            REQUIRE(pool->freeItems() == c_stCapacity);
            REQUIRE_FALSE(pool->empty());

            {
                Pool::Accessor item1 {};
                Pool::Accessor item2 {};
                REQUIRE_FALSE(static_cast<bool>(item1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(item2)); // NOLINT
                item1 = std::move(item2);
                REQUIRE_FALSE(static_cast<bool>(item1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(item2)); // NOLINT
                item2 = std::move(item1);
                REQUIRE_FALSE(static_cast<bool>(item1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(item2)); // NOLINT
                item1 = Pool::Accessor {};
                item2 = Pool::Accessor {};
                REQUIRE_FALSE(static_cast<bool>(item1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(item2)); // NOLINT
                item1 = pool->acquire();
                REQUIRE(static_cast<bool>(item1)); // NOLINT
                REQUIRE_FALSE(static_cast<bool>(item2)); // NOLINT
                item2 = std::move(item1);
                REQUIRE_FALSE(static_cast<bool>(item1)); // NOLINT
                REQUIRE(static_cast<bool>(item2)); // NOLINT
            }
        }
    }

    namespace Test4 {
        template<bool O, bool E>
        using ThrowingCtor = Ccy::DynamicResourcePool<PayloadThrowingCtor, 100, true, O, E>;

        template<bool O, bool E>
        using BasePool = Ccy::DynamicResourcePool<Payload, 100, true, O, E>;

        template<Ccy::AnyDynamicResourcePool T>
        class ThrowingPrepare : public T {
        public:
            ThrowingPrepare(T::KeyTag, Ccy::ThrowingTag, const int maxBlocks)
            : T(typename T::KeyTag {}, Ccy::ThrowingTag {}, maxBlocks) {}

            ThrowingPrepare(T::KeyTag, Ccy::NonThrowingTag, const int maxBlocks) noexcept
            : T(typename T::KeyTag {}, Ccy::NonThrowingTag {}, maxBlocks) {}

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

        template<Ccy::AnyDynamicResourcePool T>
        class ThrowingClean : public T {
        public:
            ThrowingClean(T::KeyTag, Ccy::ThrowingTag, const int maxBlocks)
            : T(typename T::KeyTag {}, Ccy::ThrowingTag {}, maxBlocks) {}

            ThrowingClean(T::KeyTag, Ccy::NonThrowingTag, const int maxBlocks) noexcept
            : T(typename T::KeyTag {}, Ccy::NonThrowingTag {}, maxBlocks) {}

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

        template<Ccy::AnyDynamicResourcePool P>
        void testFunc(int & sequence) {
            auto pool = Ccy::make<P>(10);
            if (const auto item = pool->acquire(); item) {
                item->m_arr[0] = ++sequence;
            } else {
                sequence += 3;
            }
            CHECK(Payload::s_count.load(Ccy::MemOrd::acquire) == pool->capacity());
        }

        TEST_CASE("Dynamic resource pool exception correctness check", "[throwing]") {
            int sequence { 42 };

            {
                auto func = testFunc<Ccy::DynamicResourcePool<Payload, 100, false, false, false>>;
                REQUIRE_NOTHROW(func(sequence));
                REQUIRE(sequence == 43);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<Ccy::DynamicResourcePool<Payload, 100, false, true, false>>;
                REQUIRE_NOTHROW(func(sequence));
                REQUIRE(sequence == 44);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<Ccy::DynamicResourcePool<Payload, 100, false, true, true>>;
                REQUIRE_NOTHROW(func(sequence));
                REQUIRE(sequence == 45);
            }

#if defined(__clang__)
            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<Ccy::DynamicResourcePool<Payload, c_throwingSize, false, false, false>>;
                REQUIRE_THROWS_AS(func(sequence), std::bad_alloc);
                REQUIRE(sequence == 45);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<Ccy::DynamicResourcePool<Payload, c_throwingSize, false, true, false>>;
                REQUIRE_THROWS_AS(func(sequence), std::bad_alloc);
                REQUIRE(sequence == 45);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<Ccy::DynamicResourcePool<Payload, c_throwingSize, false, true, true>>;
                REQUIRE_NOTHROW(func(sequence));
                REQUIRE(sequence == 48);
            }
#else
            sequence = 48;
#endif

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingCtor<false, false>>;
                REQUIRE_THROWS_AS(func(sequence), CtorException);
                REQUIRE(sequence == 48);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingCtor<true, false>>;
                REQUIRE_THROWS_AS(func(sequence), CtorException);
                REQUIRE(sequence == 48);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingCtor<true, true>>;
                REQUIRE_NOTHROW(func(sequence));
                REQUIRE(sequence == 51);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingPrepare<BasePool<false, false>>>;
                REQUIRE_THROWS_AS(func(sequence), PrepareException);
                REQUIRE(sequence == 51);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingPrepare<BasePool<true, false>>>;
                REQUIRE_THROWS_AS(func(sequence), PrepareException);
                REQUIRE(sequence == 51);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingPrepare<BasePool<true, true>>>;
                REQUIRE_NOTHROW(func(sequence));
                REQUIRE(sequence == 54);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingClean<BasePool<false, false>>>;
                REQUIRE_NOTHROW(func(sequence));
                REQUIRE(sequence == 55);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingClean<BasePool<true, false>>>;
                REQUIRE_NOTHROW(func(sequence));
                REQUIRE(sequence == 56);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);

            {
                auto func = testFunc<ThrowingClean<BasePool<true, true>>>;
                REQUIRE_NOTHROW(func(sequence));
                REQUIRE(sequence == 57);
            }

            REQUIRE(Payload::s_count.load(Ccy::MemOrd::acquire) == 0);
        }
    }

    namespace Test5 {
        class IntSequence : public Ccy::DynamicResourcePool<uint64_t, c_mtBlockSize, true> {
            std::atomic<Payload> m_count;

        public:
            IntSequence(KeyTag, Ccy::ThrowingTag)
            : DynamicResourcePool(KeyTag {}, Ccy::ThrowingTag {}, c_mtMaxBlocks) {}

        protected:
            bool prepare(Payload & resource) noexcept(c_noExceptAccess) override {
                resource = m_count.fetch_add(1);
                return true;
            }
        };

        static_assert(!std::default_initializable<IntSequence>);
        static_assert(!std::move_constructible<IntSequence>);
        static_assert(!std::copy_constructible<IntSequence>);
        static_assert(!std::movable<IntSequence>);
        static_assert(!std::copyable<IntSequence>);

        TEST_CASE("Concurrency workloads using a dynamic resource pool", "[concurrency]") {
            auto ints = Ccy::make<IntSequence>();
            std::vector<std::jthread> pool {};
            const unsigned workers { 2 + std::max(2u, std::thread::hardware_concurrency()) };
            std::latch latch { 1 + workers };
            std::atomic<IntSequence::Payload> result { 0 };

            for (unsigned i { workers }; i; --i) {
                pool.emplace_back(
                    [& ints, & result, & latch] {
                        for (;;) {
                            if (const auto item = ints->acquire(); item) {
                                ++*item;
                                if (*item > c_mtTestItems) {
                                    break;
                                }
                                result.fetch_add(*item, Ccy::MemOrd::acq_rel);
                            }
                        }
                        latch.count_down();
                    }
                );
            }

            latch.arrive_and_wait();

            std::cout << "Test resource pool capacity: " << ints->capacity() << std::endl;
            std::cout << "Test resource pool free items: " << ints->freeItems() << std::endl;
            std::cout << "Thread pool size: " << pool.size() << std::endl;

            REQUIRE(ints->capacity() >= c_mtBlockSize);
            REQUIRE(ints->capacity() <= c_mtCapacity);
            REQUIRE(ints->freeItems() == ints->capacity());
            REQUIRE_FALSE(ints->empty());
            REQUIRE(result.load() == c_mtExpected);

            auto a = ints->acquire();
            auto b = ints->acquire();

            REQUIRE(ints->capacity() >= c_mtBlockSize);
            REQUIRE(ints->capacity() <= c_mtCapacity);
            REQUIRE(ints->freeItems() == ints->capacity() - 2);
            REQUIRE_FALSE(ints->empty());
        }
    }
}
