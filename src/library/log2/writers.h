// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "record.h"

namespace Log {
    [[maybe_unused]] void initWriters();
    [[maybe_unused]] void reinitWriters() noexcept;
    [[maybe_unused]] void write(const Record &) noexcept;
}
