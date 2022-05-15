//
// Created by mecha on 24.04.2022.
//

#pragma once

#include <thread/Task.h>
#include <network/socket.h>

#define TCP_SERVER_INIT(listener) engine::network::tcp::Server::init(listener)
#define TCP_SERVER_LISTEN_RUN(port) engine::network::tcp::Server::listenRun(port)
#define TCP_SERVER_CLOSE() engine::network::tcp::Server::close()

#define UDP_SERVER_INIT(listener) engine::network::udp::Server::init(listener)
#define UDP_SERVER_BIND_RUN(port) engine::network::udp::Server::bindRun(port)
#define UDP_SERVER_CLOSE() engine::network::udp::Server::close()

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

        decl_exception(udp_server_exception)

        class ServerListener {
        public:
            virtual void udp_socketNotCreated() = 0;
            virtual void udp_socketClosed() = 0;
            virtual void udp_socketBindFailed() = 0;
            virtual void udp_receiveDataFailed(char* data, u16 size) = 0;
        };

        class Server final {

        private:
            Server() = default;
            ~Server() = default;

        public:
            static void init(ServerListener* serverListener);
            static void close();

            static void bind(const s32& port, const std::function<void()>& done);
            static void bindRun(const s32& port);

            static void run();
            static void stop();

        private:
            static void bindImpl(const s32& port);

            static void runImpl();

        private:
            static SOCKET clientSocket;
            static bool running;
            static thread::VoidTask<const s32&> bindTask;
            static thread::VoidTask<> runTask;
            static ServerListener* listener;
        };

    }

}
