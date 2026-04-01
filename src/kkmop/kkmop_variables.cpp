// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <constants.h>
#include <log2/variables.h>

namespace Log {
    namespace File {
        LOG_CVARE(std::wstring_view, c_filenameFormat, c_kkmOperatorFilenameFormat);
    }

    namespace EventLog {
        LOG_CVARE(wchar_t *, c_eventSource, c_kkmOperator);
    }
}
