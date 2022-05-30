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
        void onTCPSocketCreated() override;
        void onTCPSocketUnaccepted() override;
        void onTCPSocketClosed() override;
        void onTCPDisconnected() override;
        void onTCPReceiverFailed(char *data, size_t size) override;

        void onUDPSocketCreated() override;
        void onUDPSocketClosed() override;
        void onUDPConnectionFailed() override;
        void onUDPReceiverFailed(char *data, size_t size) override;
        void onUDPSenderFailed(char *data, size_t size) override;
    };

}