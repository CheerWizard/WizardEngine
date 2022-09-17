//
// Created by mecha on 30.09.2021.
//

#include <thread/Thread.h>
#include <thread>

namespace engine::thread {

    void current_sleep(const uint32_t &millis) {
        ENGINE_WARN("Sleeping current thread on {0} ms!", millis);
        std::this_thread::sleep_for(std::chrono::milliseconds(millis));
    }

    u32 cpu_cores_count() {
        return std::thread::hardware_concurrency();
    }

    u32 currentThreadId() {
        u32 threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
        return threadId;
    }
}
