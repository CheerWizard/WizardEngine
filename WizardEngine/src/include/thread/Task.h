//
// Created by mecha on 08.01.2022.
//

#pragma once

#include <io/Logger.h>
#include <thread/Thread.h>

namespace engine::thread {

    template<typename Runnable, typename Done, typename... Args>
    struct Task {
        const char* name = "Default_Task";
        const char* threadName = "Default_Thread";
        bool isRunning = false;
        std::function<Runnable> runnable;
        std::function<Done> done = [](){};
        std::function<void(Args&&...)> runImpl = [this](Args&&... args) {
            isRunning = true;
            runnable(args...);
            done();
            isRunning = false;
        };

        Task() = default;

        Task(const char* name,
             const char* threadName,
             const std::function<Runnable>& runnable,
             const std::function<Done>& done = nullptr
        ) : name(name), threadName(threadName), runnable(runnable), done(done) {}

        void run(Args &&... args) {
            if (isRunning) return;
            ENGINE_INFO("Running Task: {0}; Thread: {1}", name, threadName);
            std::thread(runImpl, args...).detach();
        }
    };

    template<typename... Args>
    struct VoidTask : public Task<void(Args...), void(), Args...> {
        VoidTask() : Task<void(Args...), void(), Args...>() {}
        VoidTask(const char* name,
                 const char* threadName,
                 const std::function<void(Args...)>& runnable = [](Args... args){},
                 const std::function<void()>& done = [](){}
        ) : Task<void(Args...), void(), Args...>(name, threadName, runnable, done) {}
    };

}
