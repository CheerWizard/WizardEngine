//
// Created by mecha on 30.09.2021.
//

#pragma once

#include <core/primitives.h>
#include <time/Time.h>

#define CURRENT_THREAD_SLEEP_TIME(time) engine::thread::current_sleep_time(time)
#define current_thread_id engine::thread::currentThreadId()

namespace engine::thread {
    using namespace core;
    ENGINE_API void current_sleep(const u32 &millis);
    ENGINE_API u32 cpu_cores_count();
    ENGINE_API u32 currentThreadId();
}
