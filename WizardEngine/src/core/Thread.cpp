//
// Created by mecha on 30.09.2021.
//

#include "Thread.h"

namespace engine {

    void Thread::current_sleep(const uint32_t &millis) {
        std::this_thread::sleep_for(MILLIS(millis));
    }

}