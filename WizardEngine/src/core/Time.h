//
// Created by mecha on 31.08.2021.
//

#pragma once

namespace engine {

    class Time {

    public:
        explicit Time(float time = 0.0f)
        : _time(time) {}

    public:
        explicit operator float() const { return _time; }

    public:
        float getSeconds() const { return _time; }
        float getMilliseconds() const { return _time * 1000.0f; }

    private:
        float _time;

    };

}