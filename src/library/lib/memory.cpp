// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "memory.h"

namespace System {
    std::optional<::DWORD> enableLowFragmentationHeap() {
        ::HANDLE heap { ::GetProcessHeap() };
        if (!heap) {
            return ::GetLastError();
        }
        ULONG HeapInformation { 2 };
        if (::HeapSetInformation(heap, HeapCompatibilityInformation, &HeapInformation, sizeof(HeapInformation))) {
            return std::nullopt;
        }
        return ::GetLastError();
    }
}
