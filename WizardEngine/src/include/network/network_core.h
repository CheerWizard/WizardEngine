//
// Created by mecha on 25.04.2022.
//

#pragma once

#include <core/exception.h>

#ifdef _WIN32

#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

#endif

#define localhost "127.0.0.1"

namespace engine::network::core {
    decl_exception(core_exception)
    void init();
    void cleanup();
}