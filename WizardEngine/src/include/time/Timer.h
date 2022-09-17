//
// Created by mecha on 03.09.2022.
//

#pragma once

#include <core/core.h>

#define invalid_limit -1

namespace engine::time {

    class ENGINE_API Timer {

    public:
        Timer(const char* functionName, float limit = invalid_limit);
        ~Timer();

    public:
        float stop();

    private:
        const char* functionName;
        long long beginTime;
        float limit = invalid_limit;
        bool stopped = false;
    };
}
// used for measuring and logging time execution of block of code
#ifdef DEBUG
#define MEASURE(...) engine::time::Timer timer##__LINE__(__FUNCSIG__, __VA_ARGS__)
#else
#define MEASURE(...)
#endif