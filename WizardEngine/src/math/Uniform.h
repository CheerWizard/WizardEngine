//
// Created by mecha on 23.09.2021.
//

#pragma once

#include "glm/glm.hpp"

namespace engine {

    template<typename V>
    struct Uniform {
        const char* name;
        V value;
        bool isUpdated = false; // used to notify Shader system that this uniform should be uploaded to GPU.

        Uniform(const char* name = "") : name(name) {
        }

        Uniform(const char* name, const V &value) :
        name(name),
        value(value) {
        }

    public:
        virtual Uniform<V>& applyChanges(); // call this method to upload your uniform on the GPU.

    };

    template<typename V>
    Uniform<V>& Uniform<V>::applyChanges() {
        isUpdated = true;
        return *this;
    }

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

}