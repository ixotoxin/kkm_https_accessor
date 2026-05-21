// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include <cassert>

namespace Ccy {
    /** Post-increment iteration **/
    template<size_t B, AnyAtomicUint T, typename U = T::value_type>
    requires (B > 1)
    U iteratePostInc(T & value) noexcept {
        U current { value.fetch_add(1, MemOrd::relaxed) };
        U next { current + 1 };
        if (next >= static_cast<U>(B)) {
#if defined(ITERATE_POST_INC_V1)
            value.compare_exchange_weak(next, next % static_cast<U>(B), MemOrd::relaxed);
#elif defined(ITERATE_POST_INC_V2)
            auto next2 = next - static_cast<U>(B);
            while (next2 >= static_cast<U>(B)) next2 -= static_cast<U>(B);
            value.compare_exchange_weak(next, next2, MemOrd::relaxed);
#else
            value.compare_exchange_weak(next, next - static_cast<U>(B), MemOrd::relaxed);
#endif /** ITERATE_POST_INC_V3 **/
        }
#if defined(ITERATE_POST_INC_V1)
        if (current >= static_cast<U>(B)) {
            current = current % static_cast<U>(B);
        }
#else /** ITERATE_POST_INC_V2, ITERATE_POST_INC_V3 **/
        while (current >= static_cast<U>(B)) current -= static_cast<U>(B);
#endif
        return current;
    }

    /** Post-increment iteration **/
    template<AnyAtomicUint T, typename U = T::value_type>
    U iteratePostInc(T & value, const U bound) noexcept {
        assert(bound > 1);
        U current { value.fetch_add(1, MemOrd::relaxed) };
        U next { current + 1 };
        if (next >= bound) {
#if defined(ITERATE_POST_INC_V1)
            value.compare_exchange_weak(next, next % bound, MemOrd::relaxed);
#elif defined(ITERATE_POST_INC_V2)
            auto next2 = next - bound;
            while (next2 >= bound) next2 -= bound;
            value.compare_exchange_weak(next, next2, MemOrd::relaxed);
#else /** ITERATE_POST_INC_V3 **/
            value.compare_exchange_weak(next, next - bound, MemOrd::relaxed);
#endif
        }
#if defined(ITERATE_POST_INC_V1)
        if (current >= bound) {
            current = current % bound;
        }
#else /** ITERATE_POST_INC_V2, ITERATE_POST_INC_V3 **/
        while (current >= bound) current -= bound;
#endif
        return current;
    }

    /** Pre-increment iteration **/
    template<size_t B, AnyAtomicUint T, typename U = T::value_type>
    requires (B > 1)
    U iteratePreInc(T & value) noexcept {
        assert(value >= 0);
        U current { value.load(MemOrd::relaxed) };
        U next;
        do {
#if defined(ITERATE_PRE_INC_V1)
            next = (current + 1) % static_cast<U>(B);
#else /** ITERATE_PRE_INC_V2 **/
            next = current + 1;
            while (next >= static_cast<U>(B)) next -= static_cast<U>(B);
#endif
        } while (!value.compare_exchange_weak(current, next, MemOrd::relaxed));
        return next;
    }

    /** Pre-increment iteration **/
    template<AnyAtomicUint T, typename U = T::value_type>
    U iteratePreInc(T & value, const U bound) noexcept {
        assert(value >= 0);
        assert(bound > 1);
        U current { value.load(MemOrd::relaxed) };
        U next;
        do {
#if defined(ITERATE_PRE_INC_V1)
            next = (current + 1) % bound;
#else /** ITERATE_PRE_INC_V2 **/
            next = current + 1;
            while (next >= bound) next -= bound;
#endif
        } while (!value.compare_exchange_weak(current, next, MemOrd::relaxed));
        return next;
    }
}
