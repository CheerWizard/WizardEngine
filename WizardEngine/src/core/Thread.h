//
// Created by mecha on 30.09.2021.
//

#pragma once

#include <cstdint>
#include <thread>
#include <chrono>

#define MILLIS(millis) std::chrono::milliseconds(millis)
#define CURRENT_THREAD_SLEEP_MILLIS(millis) engine::Thread::current_sleep(millis)

namespace engine {

    class Thread {

    public:
        static void current_sleep(const uint32_t &millis);

    };

}
