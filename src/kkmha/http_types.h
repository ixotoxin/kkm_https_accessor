// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace Http {
    using SequenceType = uint64_t;
    using IdType = uint16_t;
    using Header = std::unordered_map<std::string, std::string>;

    enum class Method { NotImplemented, Get, Post };

    enum class Status {
        Ok = 200,
        MovedTemporarily = 302,
        BadRequest = 400,
        // Unauthorized,
        Forbidden = 403,
        NotFound,
        MethodNotAllowed,
        // RequestTimeout = 408,
        // ImATeapot = 418,
        InternalServerError = 500,
        NotImplemented,
        // ServiceUnavailable = 503,
        // UnknownError = 520,
    };
}
