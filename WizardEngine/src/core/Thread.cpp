//
// Created by mecha on 30.09.2021.
//

#include "Thread.h"

#include "Logger.h"
#include <thread>

namespace engine {

    void Thread::current_sleep(const uint32_t &millis) {
        ENGINE_WARN("Sleeping current thread on {0} ms!", millis);
        std::this_thread::sleep_for(MILLIS(millis));
    }

    void Thread::current_sleep_time(const Time &time) {
        auto sleepMs = time.getLongMs();
        ENGINE_WARN("Sleeping current thread on {0} ms!", sleepMs);
        std::this_thread::sleep_for(MILLIS(sleepMs));
    }

}