//
// Created by mecha on 30.09.2021.
//

#pragma once

#include <core/primitives.h>
#include <time/Time.h>

#define CURRENT_THREAD_SLEEP_TIME(time) engine::thread::current_sleep_time(time)

namespace engine::thread {
    using namespace core;
    void current_sleep(const u32 &millis);
    void current_sleep_time(const time::Time &time);
    u32 cpu_cores_count();
}
