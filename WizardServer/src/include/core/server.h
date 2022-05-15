//
// Created by mecha on 15.05.2022.
//

#pragma once

#include <core/Application.h>

using namespace engine::core;
using namespace engine::network;

namespace server {

    class ServerApp : public tcp::ServerListener, udp::ServerListener {

    public:
        ServerApp();
        ~ServerApp();

    public:
        void update();

    public:
        void tcp_socketNotCreated() override;
        void tcp_clientSocketNotAccepted() override;
        void tcp_socketClosed() override;
        void tcp_clientDisconnected() override;
        void tcp_receiveDataFailed(char *data, u16 size) override;

        void udp_socketNotCreated() override;
        void udp_socketClosed() override;
        void udp_socketBindFailed() override;
        void udp_receiveDataFailed(char *data, u16 size) override;
    };

}