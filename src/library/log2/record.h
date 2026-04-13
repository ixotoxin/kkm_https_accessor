// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include "variables.h"
#ifndef SINGLE_THREAD
#   include <ccy/dynamique.h>
#endif
#include <string>

namespace Log {
#ifdef SINGLE_THREAD
    struct Record {
#else
    struct alignas(Ccy::c_alignment) Record {
#endif
        std::wstring m_message {};
        std::wstring_view m_terseMsg1 {};
        std::wstring_view m_terseMsg2 {};
        std::wstring m_location {};
        Category m_category { Category::Generic };
        Level m_level { Level::Debug };
        bool m_toConsole { false };
        bool m_toFile { false };
        bool m_toEventLog { false };

        Record() noexcept {
            m_message.reserve(s_lineSize);
            m_location.reserve(MAX_PATH);
        }

        Record(const Record &) = delete;
        Record(Record &&) = delete;
        virtual ~Record() = default;

        Record & operator=(const Record &) = delete;
        Record & operator=(Record &&) = delete;
    };
}
