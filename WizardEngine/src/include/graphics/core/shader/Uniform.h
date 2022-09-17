//
// Created by mecha on 23.09.2021.
//

#pragma once

#include <math/Math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace engine::shader {

    template<typename V>
    struct ENGINE_API Uniform {
        const char* name = nullptr;
        V value;

        Uniform() = default;
        Uniform(const char* name, const V& value) : name(name), value(value) {}
        Uniform(const Uniform<V>& uniform) : name(uniform.name), value(uniform.value) {}

        Uniform<V>& operator =(const V& otherValue) {
            value = otherValue;
            return *this;
        }
    };

    template<typename V>
    struct ENGINE_API UniformArray {
        const char* name = nullptr;
        std::vector<V> values {};

        UniformArray() = default;
        UniformArray(const char* name) : name(name) {}
        UniformArray(const char* name, const std::initializer_list<V>& values) : name(name), values(values) {}
        UniformArray(const UniformArray<V>& uniformArray) : name(uniformArray.name), values(uniformArray.values) {}
    };

    typedef Uniform<float> FloatUniform;
    typedef Uniform<int> IntUniform;
    typedef Uniform<bool> BoolUniform;
    typedef Uniform<double> DoubleUniform;

    typedef UniformArray<float> FloatUniformArray;
    typedef UniformArray<int> IntUniformArray;
    typedef UniformArray<bool> BoolUniformArray;
    typedef UniformArray<double> DoubleUniformArray;

    typedef Uniform<math::vec2f> Vec2fUniform;
    typedef Uniform<math::vec3f> Vec3fUniform;
    typedef Uniform<glm::vec3> GLMVec3fUniform;
    typedef Uniform<math::vec4f> Vec4fUniform;

    typedef UniformArray<math::vec2f> Vec2fUniformArray;
    typedef UniformArray<math::vec3f> Vec3fUniformArray;
    typedef UniformArray<glm::vec3> GLMVec3fUniformArray;
    typedef UniformArray<math::vec4f> Vec4fUniformArray;

    typedef Uniform<math::mat2f> Mat2fUniform;
    typedef Uniform<math::mat3f> Mat3fUniform;
    typedef Uniform<math::mat4f> Mat4fUniform;
    typedef Uniform<glm::mat4> GLMMat4fUniform;

    typedef UniformArray<math::mat2f> Mat2fUniformArray;
    typedef UniformArray<math::mat3f> Mat3fUniformArray;
    typedef UniformArray<math::mat4f> Mat4fUniformArray;
    typedef UniformArray<glm::mat4> GLMMat4fUniformArray;

    template<typename V>
    ENGINE_API inline float* glm_toFloatPtr(Uniform<V> &uniform) {
        return glm::value_ptr(uniform.value);
    }

    template<typename V>
    ENGINE_API inline float* toFloatPtr(Uniform<V> &uniform) {
        return math::values(uniform.value);
    }

    template<typename V>
    ENGINE_API inline int* toIntPtr(Uniform<V> &uniform) {
        return math::values(uniform.value);
    }
}
