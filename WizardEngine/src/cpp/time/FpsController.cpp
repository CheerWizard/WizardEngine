//
// Created by mecha on 03.10.2021.
//

#include <time/FpsController.h>
#include <io/Logger.h>

namespace engine::time {

    void FpsController::begin() {
        ENGINE_INFO("FpsController: begin()");
        _beginTime = CURRENT_TIME_MS.count();
    }

    void FpsController::end() {
        ENGINE_INFO("FpsController: end()");
        float endTime = CURRENT_TIME_MS.count();

        ENGINE_INFO("Begin time: {0}ms! End time: {1}ms!", _beginTime, endTime);

        _deltaTime.setMs(endTime - _beginTime);

        auto minDeltaTime = getMinDeltaTime();
        if (_deltaTime < minDeltaTime) {
            CURRENT_THREAD_SLEEP_TIME(minDeltaTime - _deltaTime);
            _deltaTime = minDeltaTime;
        }

        ENGINE_INFO("FPS: {0}; dt: {1} ms", 1.0f / _deltaTime, _deltaTime.getMilliseconds());
    }

}