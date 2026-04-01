// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <log2/core.h>

namespace Server {
    template<Meta::View T, typename ... Args>
    [[maybe_unused]]
    void log(const Log::Level level, const T message, Args && ... args) noexcept {
        Log::write<typename Meta::TextTrait<T>::View>(
            Log::Category::WebServer, level, {}, message, std::forward<Args>(args)...
        );
    }

    template<Meta::Char T, typename ... Args>
    [[maybe_unused]]
    void log(const Log::Level level, const T * message, Args && ... args) noexcept {
        Log::write<typename Meta::TextTrait<T>::View>(
            Log::Category::WebServer, level, {}, message, std::forward<Args>(args)...
        );
    }

    template<Meta::String T, typename ... Args>
    [[maybe_unused]]
    void log(const Log::Level level, const T & message, Args && ... args) noexcept {
        Log::write<typename Meta::TextTrait<T>::View>(
            Log::Category::WebServer, level, {}, message, std::forward<Args>(args)...
        );
    }

    [[maybe_unused]]
    inline void log(const Log::Level level, const Basic::Failure & e) noexcept {
        Log::write(Log::Category::WebServer, level, {}, e);
    }
}
