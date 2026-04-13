// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <cstdint>
#include <limits>
#include <exception>
#include <atomic>
#include <string>
#include <chrono>
#include <ccy/types.h>

namespace UnitTests {
    using namespace std::string_literals;
    using namespace std::chrono_literals;

    constexpr int c_stBlockSize { 1'000 };
    constexpr int c_stMaxBlocks { 10 };
    constexpr int c_stCapacity { c_stBlockSize * c_stMaxBlocks };
    constexpr int c_stOvercapacity  { c_stBlockSize };

    namespace Test1 {
        using Payload = int;
    }

    namespace Test2 {
        struct Payload {
            std::string m_str {};
            int m_int { 0 };
            void setBool(const bool val) { m_bool = val; }
            [[nodiscard]] bool getBool() const { return m_bool; }

        private:
            bool m_bool {};
        };

        constexpr int c_stMagicNumber { 777 };
    }

    namespace Test3 {
        using Payload = int;
    }

    namespace Test4 {
        constexpr int c_throwingSize { std::numeric_limits<int>::max() };

        class CtorException : public std::exception { public: CtorException() : exception("ctor") {} };
        class PrepareException : public std::exception { public: PrepareException() : exception("prepare") {} };
        class CleanException : public std::exception { public: CleanException() : exception("clear") {} };

        template<class T>
        void throwingFunc() {
            throw T {};
        }

        struct Payload {
            static inline std::atomic_int64_t s_count { 0 };
            int m_arr[1024 / sizeof(int)] {};
            Payload() {
                s_count.fetch_add(1, Ccy::MemOrd::acq_rel);
                for (auto & el : m_arr) {
                    el = std::numeric_limits<int>::max();
                }
            }
            ~Payload() {
                s_count.fetch_sub(1, Ccy::MemOrd::acq_rel);
            }
        };

        struct PayloadThrowingCtor : Payload {
            PayloadThrowingCtor() {
                if (s_count.load(Ccy::MemOrd::acquire) == 42) {
                    throw CtorException {};
                }
            }
        };
    }

    constexpr int c_mtBlockSize { 500 };
    constexpr int c_mtMaxBlocks { 10 };
    constexpr int c_mtCapacity { c_mtBlockSize * c_mtMaxBlocks };
    constexpr auto c_mtTimeout = 15s;

    namespace Test5 {
        using Payload = int64_t;
        constexpr Payload c_mtTestItems { 5'000'000 };
        constexpr Payload c_mtExpected { (c_mtTestItems * (c_mtTestItems + 1)) >> 1 };
    }

    namespace Test6 {
        using Payload = int64_t;
        constexpr Payload c_mtTestItems { 5'000'000 };
        constexpr Payload c_mtMagicNumber1 { c_mtCapacity / 2 };
        constexpr Payload c_mtMagicNumber2 { 4'500'000 };
        constexpr Payload c_mtExpected {
            (c_mtMagicNumber2 * (c_mtMagicNumber2 + 1) - c_mtMagicNumber1 * (c_mtMagicNumber1 + 1)) >> 1
        };
    }
}
