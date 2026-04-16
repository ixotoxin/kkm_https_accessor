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
    // v1 {{{
            // value.compare_exchange_weak(next, next % static_cast<U>(B), MemOrd::relaxed);
    // }}} v1
    // v2 {{{
            // auto next2 = next - static_cast<U>(B);
            // while (next2 >= static_cast<U>(B)) next2 -= static_cast<U>(B);
            // value.compare_exchange_weak(next, next2, MemOrd::relaxed);
    // }}} v2
    // v3 {{{
            value.compare_exchange_weak(next, next - static_cast<U>(B), MemOrd::relaxed);
    // }}}
        }
    // v1 {{{
        // if (current >= static_cast<U>(B)) {
        //     current = current % static_cast<U>(B);
        // }
    // }}} v1
    // v2,3 {{{
        while (current >= static_cast<U>(B)) current -= static_cast<U>(B);
    // }}} v2,3
        return current;
    }

    /** Post-increment iteration **/
    template<AnyAtomicUint T, typename U = T::value_type>
    U iteratePostInc(T & value, const U bound) noexcept {
        assert(bound > 1);
        U current { value.fetch_add(1, MemOrd::relaxed) };
        U next { current + 1 };
        if (next >= bound) {
    // v1 {{{
            // value.compare_exchange_weak(next, next % bound, MemOrd::relaxed);
    // }}} v1
    // v2 {{{
            // auto next2 = next - bound;
            // while (next2 >= bound) next2 -= bound;
            // value.compare_exchange_weak(next, next2, MemOrd::relaxed);
    // }}} v2
    // v3 {{{
            value.compare_exchange_weak(next, next - bound, MemOrd::relaxed);
    // }}} v3
        }
    // v1 {{{
        // if (current >= bound) {
        //     current = current % bound;
        // }
    // }}} v1
    // v2,3 {{{
        while (current >= bound) current -= bound;
    // }}} v2,3
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
    // v1 {{{
            // next = (current + 1) % static_cast<U>(B);
    // }}} v1
    // v2 {{{
            next = current + 1;
            while (next >= static_cast<U>(B)) next -= static_cast<U>(B);
    // }}} v2
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
    // v1 {{{
            // next = (current + 1) % bound;
    // }}} v1
    // v2 {{{
            next = current + 1;
            while (next >= bound) next -= bound;
    // }}} v2
        } while (!value.compare_exchange_weak(current, next, MemOrd::relaxed));
        return next;
    }
}
