//
// Created by mecha on 25.04.2022.
//

#pragma once

#include <thread/Task.h>
#include <network/socket.h>

namespace engine::network::tcp {

    using namespace engine::core;

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
