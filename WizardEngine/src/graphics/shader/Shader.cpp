//
// Created by mecha on 05.09.2021.
//

#include "Shader.h"

namespace engine {

    uint32_t Shader::addAttribute(Attribute* attribute) {
        return vertex->add(attribute);
    }

    void Shader::replaceAttribute(const uint32_t &index, Attribute* attribute) {
        vertex->replace(index, attribute);
    }

    Attribute &Shader::getAttribute(const uint32_t &index) const {
        return vertex->get(index);
    }

    void Shader::removeAttribute(const uint32_t &index) {
        vertex->remove(index);
    }

}