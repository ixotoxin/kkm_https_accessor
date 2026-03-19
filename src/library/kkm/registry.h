// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "connparams.h"
#include "device.h"

namespace Kkm::Registry {
    [[nodiscard]] std::wstring serialNumber(const std::filesystem::path &);
    [[nodiscard]] std::filesystem::path filePath(const std::wstring &, bool = false);
    [[nodiscard]] ConnParams make(const std::wstring &);
    [[nodiscard]] ConnParams load(const std::wstring &);
    [[nodiscard]] ConnParams read(const std::filesystem::path &, const std::wstring &);
    void save(const ConnParams &, Device &);
}
