// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "server_defaults.h"
#include <filesystem>

namespace Server {
    inline int64_t s_requestTimeout { c_defRequestTimeout };
    inline bool s_ipv4Only { c_defIpv4Only };
    inline unsigned short s_port { c_defPort };
    inline size_t s_poolSize { c_defPoolSize };
    inline int64_t s_concurrencyLimit { c_defConcurrencyLimit };
    inline bool s_enableLegacyTls { false };
    inline int s_securityLevel { -1 };
    inline std::filesystem::path s_certificateChainFile { c_defCertificateChainFile };
    inline std::filesystem::path s_privateKeyFile { c_defPrivateKeyFile };
    inline std::string s_privateKeyPassword {};
    inline std::string s_secret { c_defSecret };
    inline bool s_loopbackWithoutSecret { c_loopbackWithoutSecret };
}
