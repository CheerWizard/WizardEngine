//
// Created by mecha on 23.09.2021.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    typedef Uniform<glm::fvec2> Vec2fUniform;
    typedef Uniform<glm::fvec3> Vec3fUniform;
    typedef Uniform<glm::fvec4> Vec4fUniform;

    typedef Uniform<glm::fmat2> Mat2fUniform;
    typedef Uniform<glm::fmat3> Mat3fUniform;
    typedef Uniform<glm::fmat4> Mat4fUniform;

    template<typename V>
    inline float* toFloatPtr(Uniform<V> &uniform) {
        return glm::value_ptr(uniform.value);
    }

    template<typename V>
    inline int* toIntPtr(Uniform<V> &uniform) {
        return glm::value_ptr(uniform.value);
    }
}
