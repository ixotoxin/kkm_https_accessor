// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "spinlock.h"
#include "dynrespool.h"
#include "dynamique.h"
#include "statique.h"
#include <type_traits>
#include <mutex>

namespace Ccy {
    template<typename L>
    concept AnyLocker
        = std::is_same_v<L, SpinLock<SpinMethod::Pause>>
        || std::is_same_v<L, SpinLock<SpinMethod::YieldThread>>
        || std::is_same_v<L, SpinLock<SpinMethod::WaitFlag>>
        || std::is_same_v<L, SpinLock<SpinMethod::Active>>
        || std::is_same_v<L, std::mutex>
        || std::is_same_v<L, std::recursive_mutex>;

    template<typename P>
    concept AnyResourcePool = AnyDynamicResourcePool<P>;

    template<typename Q>
    concept AnyMpmcQueue = AnyDynamicMpmcQueue<Q> || AnyStaticMpmcQueue<Q>;
}
