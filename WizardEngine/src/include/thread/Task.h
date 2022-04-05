//
// Created by mecha on 08.01.2022.
//

#pragma once

#include <io/Logger.h>
#include <thread/Thread.h>

namespace engine::thread {

    template<typename Runnable, typename Callback, typename... Args>
    struct Task {
        const char* name = "Default_Task";
        const char* threadName = "Default_Thread";
        bool isRunning = false;
        std::function<Runnable> runnable;
        std::function<Callback> callback = nullptr;

        Task(const char* name,
             const char* threadName,
             const std::function<Runnable>& runnable,
             const std::function<Callback>& callback = nullptr
        ) : name(name), threadName(threadName), runnable(runnable), callback(callback) {}

        void run(Args&&... args) {
            if (isRunning) return;
            isRunning = true;

            ENGINE_INFO("Running Task: {0}; Thread: {1}", name, threadName);
            std::thread thread(runnable, std::forward<Args>(args)...);
            thread.join();
            isRunning = false;
            ENGINE_INFO("Finished Task: {0}; Joined Thread: {1}", name, threadName);
            if (callback != nullptr) {
                callback();
            }
        }
    };

    template<typename... Args>
    struct VoidTask : public Task<void(Args...), void(), Args...> {
        VoidTask(const char* name,
                 const char* threadName,
                 const std::function<void(Args...)>& runnable,
                 const std::function<void()>& callback = nullptr
        ) : Task<void(Args...), void(), Args...>(name, threadName, runnable, callback) {}
    };

}
