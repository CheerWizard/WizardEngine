//
// Created by mecha on 30.09.2021.
//

#pragma once

#include "Time.h"

#include <cstdint>

#define CURRENT_THREAD_SLEEP_MILLIS(millis) engine::Thread::current_sleep(millis)
#define CURRENT_THREAD_SLEEP_TIME(time) engine::Thread::current_sleep_time(time)

namespace engine {

    class Thread {

    public:
        static void current_sleep(const uint32_t &millis);
        static void current_sleep_time(const Time &time);

    };

}
