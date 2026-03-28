// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "spinlock.h"
#include <type_traits>
#include <mutex>

namespace Ccy {
    template<typename T>
    concept Locker
        = std::is_same_v<T, SpinLock<Spin::Pause>>
        || std::is_same_v<T, SpinLock<Spin::YieldThread>>
        || std::is_same_v<T, SpinLock<Spin::WaitFlag>>
        || std::is_same_v<T, SpinLock<Spin::Active>>
        || std::is_same_v<T, std::mutex>
        || std::is_same_v<T, std::recursive_mutex>;
}
