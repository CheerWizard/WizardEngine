//
// Created by mecha on 08.01.2022.
//

#pragma once

#include "Logger.h"
#include "Thread.h"

namespace engine {

    template<typename Fn, typename... Args>
    struct Task {
        const char* name = "Default_Task";
        const char* threadName = "Default_Thread";
        bool isRunning = false;
        std::function<Fn> runnable;

        Task(const char* name, const char* threadName, const std::function<Fn>& runnable)
        : name(name), threadName(threadName), runnable(runnable) {}

        void run(Args&&... args) {
            if (isRunning) return;
            isRunning = true;

            ENGINE_INFO("Running Task: {0}; Thread: {1}", name, threadName);
            std::thread thread(runnable, std::forward<Args>(args)...);
            thread.join();
            isRunning = false;
            ENGINE_INFO("Finished Task: {0}; Joined Thread: {1}", name, threadName);
        }
    };

}
