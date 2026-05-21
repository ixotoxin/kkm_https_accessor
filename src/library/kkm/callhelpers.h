// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "device.h"
#include "impex.h"
#include <cassert>
#include <type_traits>

namespace Kkm {
    template<std::derived_from<Result> R>
    using UndetailedMethod = void (Device::*)(R &);

    template<class R, class K>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void callMethod(K && kkm, UndetailedMethod<R> method, JsonDoc & result) {
        assert(result.IsObject());
        R callResult {};
        (kkm.*method)(callResult);
        result <<= callResult;
    }

    template<class K, typename ... METHODS>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void collectDataFromMethods(JsonDoc & result, K && kkm, METHODS ... method) {
        (callMethod(kkm, method, result), ...);
    }

    template<std::derived_from<Result> R, std::derived_from<Details> D>
    using DetailedMethod = void (Device::*)(const D &, R &);

    template<class R, class D, class K>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void callMethod(K && kkm, DetailedMethod<R, D> method, const D & details, JsonDoc & result) {
        assert(result.IsObject());
        R callResult {};
        (kkm.*method)(details, callResult);
        result <<= callResult;
    }

    template<class R, class D, class K>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void callMethod(K && kkm, DetailedMethod<R, D> method, const JsonDoc & details, JsonDoc & result) {
        assert(details.IsObject());
        assert(result.IsObject());
        D callDetails {};
        details >>= callDetails;
        callMethod(std::forward<K>(kkm), method, callDetails, result);
    }
}
