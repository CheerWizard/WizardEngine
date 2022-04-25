//
// Created by mecha on 24.04.2022.
//

#pragma once

#include <thread/Task.h>
#include <network/socket.h>

namespace engine::network::tcp {

    using namespace engine::core;

    decl_exception(tcp_server_exception)

    struct ClientProfile {
        SOCKET socket;
        const char* host;
        const char* service;
    };

    class Server final {

    private:
        Server() = default;
        ~Server() = default;

    public:
        static void init();
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
    };

}
