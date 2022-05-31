//
// Created by mecha on 24.04.2022.
//

#pragma once

#include <thread/Task.h>
#include <network/socket.h>
#include <network/gdp.h>

namespace engine::network {

    using namespace engine::core;

    struct ClientProfile {
        SOCKET socket;
        const char* host;
        const char* service;
    };

    namespace tcp {

        class ServerListener {
        public:
            virtual void onTCPSocketCreated() = 0;
            virtual void onTCPSocketUnaccepted() = 0;
            virtual void onTCPSocketClosed() = 0;
            virtual void onTCPDisconnected() = 0;
            virtual void onTCPReceiverFailed(char* data, size_t size) = 0;
        };

        class Server final {

        public:
            static bool init(ServerListener* serverListener);
            static void close();

            static void listen(const s32& port);
            static void stop();

        private:
            static void listenImpl(const s32& port);
            static void runImpl();

        private:
            static SOCKET listeningSocket;
            static ClientProfile clientProfile;
            static thread::VoidTask<const s32&> listenTask;
            static ServerListener* listener;
        };
    }

    namespace udp {

        class ServerListener {
        public:
            virtual void onUDPSocketCreated() = 0;
            virtual void onUDPSocketClosed() = 0;
            virtual void onUDPConnectionFailed() = 0;
            virtual void onUDPReceiverFailed(char* data, size_t size) = 0;
            virtual void onUDPSenderFailed(char* data, size_t size) = 0;
        };

        class Server final {

        private:
            Server() = default;
            ~Server() = default;

        public:
            static bool init(ServerListener* serverListener);
            static void close();

            static void listen(const s32& port);
            static void stop();

        private:
            static void listenImpl(const s32& port);
            static void runImpl();
            static void dispatch(char* data, u32 type);

            static void saveWorld(char* data);
            static void loadWorld();
            static void send(char* data, size_t size);

        private:
            static SOCKET clientSocket;
            static thread::VoidTask<const s32&> listenTask;
            static ServerListener* listener;
            static sockaddr_in client;
        };

    }

}
