//
// Created by mecha on 25.04.2022.
//

#pragma once

#include <thread/Task.h>
#include <network/socket.h>

#define TCP_CLIENT_INIT(listener) engine::network::tcp::Client::init(listener)
#define TCP_CLIENT_CONNECT_RUN(ip, port) engine::network::tcp::Client::connectRun(ip, port)
#define TCP_CLIENT_CLOSE() engine::network::tcp::Client::close()

#define UDP_CLIENT_INIT(listener) engine::network::udp::Client::init(listener)
#define UDP_CLIENT_BIND(ip, port) engine::network::udp::Client::bind(ip, port)
#define UDP_CLIENT_BIND_CALLBACK(ip, port, callback) engine::network::udp::Client::bind(ip, port, callback)
#define UDP_CLIENT_SEND(data) engine::network::udp::Client::send(data)
#define UDP_CLIENT_CLOSE() engine::network::udp::Client::close()

namespace engine::network {

    using namespace engine::core;

    namespace tcp {

        decl_exception(tcp_client_exception)

        class ClientListener {
        public:
            virtual void tcp_socketNotCreated() = 0;
            virtual void tcp_connectionFailed() = 0;
            virtual void tcp_socketClosed() = 0;
        };

        class Client final {

        private:
            Client() = default;
            ~Client() = default;

        public:
            static void init(ClientListener* clientListener);
            static void close();

            static void connectRun(const std::string& ip, const s32& port);
            static void connect(const std::string& ip, const s32& port, const std::function<void()>& done);

            static void run();
            static void stop();

        private:
            static void connectImpl(const std::string& ip, const s32& port);

            static void runImpl();

        private:
            static SOCKET clientSocket;
            static bool running;
            static thread::VoidTask<const std::string&, const s32&> connectionTask;
            static thread::VoidTask<> runTask;
            static ClientListener* listener;
        };
    }

    namespace udp {

        decl_exception(udp_client_exception)

        class ClientListener {
        public:
            virtual void udp_socketNotCreated() = 0;
            virtual void udp_sendDataFailed(const std::string& data) = 0;
            virtual void udp_socketClosed() = 0;
        };

        class Client final {

        private:
            Client() = default;
            ~Client() = default;

        public:
            static void init(ClientListener* clientListener);
            static void close();

            static void bind(const std::string& ip, const s32& port);
            static void bind(const std::string& ip, const s32& port, const std::function<void()>& done);

            static void send(const std::string& data);

        private:
            static void bindImpl(const std::string& ip, const s32& port);

            static void sendImpl(const std::string& data);

        private:
            static SOCKET clientSocket;
            static thread::VoidTask<const std::string&, const s32&> bindTask;
            static thread::VoidTask<const std::string&> sendTask;
            static ClientListener* listener;
        };
    }

}
