//
// Created by mecha on 03.10.2021.
//

#include "Timer.h"
#include "Logger.h"

namespace engine {

    void Timer::begin() {
        ENGINE_INFO("Timer : begin()");
        _beginTime = CURRENT_TIME_MS.count();
    }

    void Timer::end() {
        ENGINE_INFO("Timer : end()");
        float endTime = CURRENT_TIME_MS.count();

        ENGINE_INFO("Begin time : {0} ; End time : {1}", _beginTime, endTime);

        _deltaTime.setMs(endTime - _beginTime);

        auto minDeltaTime = getMinDeltaTime();
        if (_deltaTime < minDeltaTime) {
            CURRENT_THREAD_SLEEP_TIME(minDeltaTime - _deltaTime);
            _deltaTime = minDeltaTime;
        }

        ENGINE_INFO("FPS : {0} ; delta time : {1} seconds!", 1.0f / _deltaTime, _deltaTime.getSeconds());
    }

}