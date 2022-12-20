#pragma once

#include <core/primitives.h>
#include <time/Time.h>

#include <future>

#define CURRENT_THREAD_SLEEP_TIME(time) engine::thread::current_sleep_time(time)
#define current_thread_id engine::thread::currentThreadId()

namespace engine::thread {

    using namespace core;

    enum ThreadPriority {
        LOWEST, NORMAL, HIGHEST
    };

    struct ENGINE_API ThreadFormat {
        ThreadPriority priority;
        const char* name;

        ThreadFormat(ThreadPriority newPriority, const char* newName)
        : priority(newPriority), name(newName) {}
    };

    ENGINE_API void current_sleep(const u32 &millis);
    ENGINE_API u32 cpu_cores_count();
    ENGINE_API u32 currentThreadId();
    ENGINE_API void setThreadFormat(u32 id, std::thread& thread, const ThreadFormat& format);
}
