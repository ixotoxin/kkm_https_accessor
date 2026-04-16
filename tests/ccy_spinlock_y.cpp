// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <mutex>
#include <latch>
#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <ccy/spinlock.h>
#include "ccy_common.h"

namespace UnitTests {
    namespace Test1 {
        TEST_CASE("Ccy::SpinLock<Ccy::SpinMethod::YieldThread> try_lock/unlock", "[operating]") {
            Ccy::SpinLock<Ccy::SpinMethod::YieldThread> spinLock;
            REQUIRE(spinLock.try_lock());
            REQUIRE_FALSE(spinLock.try_lock());
            spinLock.unlock();
            REQUIRE(spinLock.try_lock());
            REQUIRE_FALSE(spinLock.try_lock());
        }
    }

    namespace Test2 {
        TEST_CASE("Ccy::SpinLock<Ccy::SpinMethod::YieldThread> with std::scoped_lock", "[operating]") {
            Ccy::SpinLock<Ccy::SpinMethod::YieldThread> spinLock;
            REQUIRE(spinLock.try_lock());
            spinLock.unlock();
            {
                spinLock.unlock();
                std::scoped_lock lock { spinLock };
                REQUIRE_FALSE(spinLock.try_lock());
            }
            REQUIRE(spinLock.try_lock());
        }
    }

    namespace Test5 {
        TEST_CASE("Concurrency workloads using a Ccy::SpinLock<Ccy::SpinMethod::YieldThread>", "[concurrency]") {
            std::vector<std::jthread> pool {};
            const unsigned workers { std::max(4u, std::thread::hardware_concurrency()) - 2 };
            std::latch latch { 1 + workers };
            Payload counter { c_mtTestItems };
            Payload result { 0 };
            Ccy::SpinLock<Ccy::SpinMethod::YieldThread> spinLock {};

            for (unsigned i { workers }; i; --i) {
                pool.emplace_back(
                    [& spinLock, & counter, & result, & latch] {
                        for (;;) {
                            std::scoped_lock lock { spinLock };
                            if (!counter) {
                                break;
                            }
                            result += counter--;
                        }
                        latch.count_down();
                    }
                );
            }

            latch.arrive_and_wait();

            REQUIRE(counter == 0);
            REQUIRE(result == c_mtExpected);
        }
    }
}
