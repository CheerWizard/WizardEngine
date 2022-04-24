//
// Created by mecha on 24.04.2022.
//

#pragma once

#include <thread/Task.h>

namespace engine::network {

    class Server final {

    private:
        Server() = default;
        ~Server() = default;

    public:
        static void run();
        static void stop();

    private:
        static void runImpl();

    private:
        static bool running;
        static thread::VoidTask<> runTask;
    };

}
