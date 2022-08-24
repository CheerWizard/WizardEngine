//
// Created by mecha on 30.09.2021.
//

#include <thread/Thread.h>
#include <io/Logger.h>
#include <thread>

namespace engine::thread {

    void current_sleep(const uint32_t &millis) {
        ENGINE_WARN("Sleeping current thread on {0} ms!", millis);
        std::this_thread::sleep_for(MILLIS(millis));
    }

    void current_sleep_time(const time::Time &time) {
        current_sleep(time.getLongMs());
    }

    u32 cpu_cores_count() {
        return std::thread::hardware_concurrency();
    }
}
