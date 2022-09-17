//
// Created by mecha on 31.08.2021.
//

#pragma once

#include <chrono>
#include <math/Math.h>

#define MILLIS(millis) time::ms(millis)
#define NANO_UNIT 1.0E-9
#define MILLI_UNIT 1000.0f

namespace engine::time {

    using namespace math;

    class ENGINE_API Time {

    public:
        Time(float ms = 6) : ms(ms) {}

    public:
        [[nodiscard]] float milliseconds() const;
        [[nodiscard]] float seconds() const;

    private:
        float ms;

    public:
        friend float operator /(float v, const Time& time) {
            return v / time.milliseconds();
        }

        friend float operator *(float v, const Time& time) {
            return v * time.milliseconds();
        }

        explicit operator float() const {
            return ms;
        }

        template<typename T>
        friend vec3<T> operator *(const vec3<T>& v, const Time& time) {
            return v * static_cast<T>(time.milliseconds());
        }
    };

}