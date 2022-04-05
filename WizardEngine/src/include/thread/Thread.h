//
// Created by mecha on 30.09.2021.
//

#pragma once

#include <time/Time.h>

#include <cstdint>

#define CURRENT_THREAD_SLEEP_TIME(time) engine::thread::current_sleep_time(time)

namespace engine::thread {

    static void current_sleep(const uint32_t &millis);
    static void current_sleep_time(const time::Time &time);

}
