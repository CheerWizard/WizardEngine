//
// Created by mecha on 23.09.2021.
//

#pragma once

#include "string"
#include "glm/glm.hpp"

namespace engine {

    template<typename V>
    struct Uniform {
        const char* name;
        V value;

        Uniform(const char* name) : name(name) {}

        Uniform(const char* name, const V &value) :
        name(name),
        value(value) {}

    public:
        virtual const V& applyChanges();

    };

    template<typename V>
    const V &Uniform<V>::applyChanges() {
        return value;
    }

    typedef Uniform<glm::fmat4> Mat4fUniform;

}