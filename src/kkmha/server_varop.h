// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "server_json.h"
#include <ostream>

namespace Server {
    void setVars(const JsonVal &);
    std::wostream & vars(std::wostream &);
}
