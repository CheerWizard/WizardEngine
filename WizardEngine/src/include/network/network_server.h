//
// Created by mecha on 24.04.2022.
//

#pragma once

#include <thread/Task.h>
#include <network/socket.h>
#include <network/gdp.h>

#define TCP_SERVER_INIT(listener) engine::network::tcp::Server::init(listener)
#define TCP_SERVER_LISTEN_RUN(port) engine::network::tcp::Server::listenRun(port)
#define TCP_SERVER_CLOSE() engine::network::tcp::Server::close()

namespace engine::network {

    using namespace engine::core;

    struct ClientProfile {
        SOCKET socket;
        const char* host;
        const char* service;
    };

    namespace tcp {

        decl_exception(tcp_server_exception)

        class ServerListener {
        public:
            virtual void tcp_socketNotCreated() = 0;
            virtual void tcp_clientSocketNotAccepted() = 0;
            virtual void tcp_socketClosed() = 0;
            virtual void tcp_clientDisconnected() = 0;
            virtual void tcp_receiveDataFailed(char* data, u16 size) = 0;
        };

        class Server final {

        private:
            Server() = default;
            ~Server() = default;

        public:
            static void init(ServerListener* serverListener);
            static void close();

            static void listen(const s32& port, const std::function<void()>& done);
            static void listenRun(const s32& port);

            static void run();
            static void stop();

        private:
            static void listenImpl(const s32& port);

            static void runImpl();

        private:
            static SOCKET listeningSocket;
            static ClientProfile clientProfile;
            static bool running;
            static thread::VoidTask<const s32&> listenTask;
            static thread::VoidTask<> runTask;
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

        private:
            static SOCKET clientSocket;
            static thread::VoidTask<const s32&> listenTask;
            static ServerListener* listener;
        };

    }

}
