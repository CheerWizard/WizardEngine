//
// Created by mecha on 23.09.2021.
//

#pragma once

#include <math/Math.h>

namespace engine::shader {

    template<typename V>
    struct Uniform {
        const char* name = nullptr;
        V value;
        bool isUpdated = true; // flag to minimize GPU calls.

        Uniform() = default;
        Uniform(const char* name, const V& value) : name(name), value(value) {}
        Uniform(const char* name, const V& value, const bool& isUpdated) : name(name), value(value), isUpdated(isUpdated) {}
        Uniform(const Uniform<V>& uniform) : name(uniform.name), value(uniform.value), isUpdated(uniform.isUpdated) {}
    };

    typedef Uniform<float> FloatUniform;
    typedef Uniform<int> IntUniform;
    typedef Uniform<bool> BoolUniform;
    typedef Uniform<double> DoubleUniform;

    typedef Uniform<math::vec2f> Vec2fUniform;
    typedef Uniform<math::vec3f> Vec3fUniform;
    typedef Uniform<math::vec4f> Vec4fUniform;

    typedef Uniform<math::mat2f> Mat2fUniform;
    typedef Uniform<math::mat3f> Mat3fUniform;
    typedef Uniform<math::mat4f> Mat4fUniform;

    template<typename V>
    inline float* toFloatPtr(Uniform<V> &uniform) {
        return math::values(uniform.value);
    }

    template<typename V>
    inline int* toIntPtr(Uniform<V> &uniform) {
        return math::values(uniform.value);
    }
}
