//
// Created by mecha on 25.04.2022.
//

#pragma once

#include <thread/Task.h>
#include <network/socket.h>

#define TCP_CLIENT_INIT() engine::network::tcp::Client::init()
#define TCP_CLIENT_CONNECT_RUN(ip, port) engine::network::tcp::Client::connectRun(ip, port)
#define TCP_CLIENT_CLOSE() engine::network::tcp::Client::close()

#define UDP_CLIENT_INIT() engine::network::udp::Client::init()
#define UDP_CLIENT_BIND(ip, port) engine::network::udp::Client::bind(ip, port)
#define UDP_CLIENT_BIND_CALLBACK(ip, port, callback) engine::network::udp::Client::bind(ip, port, callback)
#define UDP_CLIENT_SEND(data) engine::network::udp::Client::send(data)
#define UDP_CLIENT_CLOSE() engine::network::udp::Client::close()

namespace engine::network {

    using namespace engine::core;

    namespace tcp {

        decl_exception(tcp_client_exception)

        class Client final {

        private:
            Client() = default;
            ~Client() = default;

        public:
            static void init();
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
        };
    }

    namespace udp {

        decl_exception(udp_client_exception)

        class Client final {

        private:
            Client() = default;
            ~Client() = default;

        public:
            static void init();
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
        };
    }

}
