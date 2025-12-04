// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#if WITH_SHORTCUTS
#   define KKM_CMD_EQ(COMMAND, VALUE, SHORTCUT) (((COMMAND) == (VALUE)) || ((COMMAND) == (SHORTCUT)))
#else
#   define KKM_CMD_EQ(COMMAND, VALUE, SHORTCUT) ((COMMAND) == (VALUE))
#endif
