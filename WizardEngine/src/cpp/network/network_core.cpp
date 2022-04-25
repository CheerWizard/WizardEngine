//
// Created by mecha on 25.04.2022.
//

#include <network/network_core.h>
#include <io/Logger.h>

namespace engine::network::core {

    using namespace engine::core;

    void init() {
        WSAData wsaData;
        WORD version = MAKEWORD(2, 2);
        s32 wsInit = WSAStartup(version, &wsaData);
        if (wsInit != 0) {
            ENGINE_ERR("Unable to initialize win socket version: {0}", version);
            ENGINE_THROW(core_exception("Unable to initialize network core!"));
        }
    }

    void cleanup() {
        WSACleanup();
    }

}