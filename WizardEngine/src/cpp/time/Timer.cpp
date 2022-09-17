//
// Created by mecha on 03.09.2022.
//

#include <time/Timer.h>
#include <io/Logger.h>

#include <chrono>

namespace engine::time {

    using namespace std::chrono;

    Timer::Timer(const char *functionName, float limit) : functionName(functionName), limit(limit) {
        auto beginTimePoint = high_resolution_clock::now();
        beginTime = time_point_cast<microseconds>(beginTimePoint).time_since_epoch().count();
    }

    Timer::~Timer() {
        if (!stopped) {
            stop();
        }
    }

    float Timer::stop() {
        stopped = true;

        auto endTimePoint = high_resolution_clock::now();
        long long endTime = time_point_cast<microseconds>(endTimePoint).time_since_epoch().count();
        float dt = (endTime - beginTime) * 0.001f;

        if (limit == invalid_limit) {
            ENGINE_INFO("{0} took {1} ms, FPS: {2}", functionName, dt, 1 / dt * 1000);
        } else if (dt > limit) {
            ENGINE_ERR("{0} took {1} ms, FPS: {2}, limit: {3}", functionName, dt, 1 / dt * 1000, limit);
        } else {
            ENGINE_INFO("{0} took {1} ms, FPS: {2}, limit: {3}", functionName, dt, 1 / dt * 1000, limit);
        }

        return dt;
    }

}