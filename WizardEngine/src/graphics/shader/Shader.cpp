//
// Created by mecha on 05.09.2021.
//

#include "Shader.h"

namespace engine {

    const Attribute& Shader::getAttribute(const uint32_t &index) const {
        return vertexFormat->get(index);
    }

    void Shader::removeAttribute(const uint32_t &index) {
        vertexFormat->remove(index);
    }

    std::string Shader::readShader(const std::string &name) {
        setAssetName(name);
        return read();
    }

    uint32_t Shader::addAttribute(const Attribute &attribute) {
        return vertexFormat->add(attribute);
    }

    void Shader::replaceAttribute(const uint32_t &index, const Attribute &attribute) {
        vertexFormat->replace(index, attribute);
    }

    const char *Shader::getAssetPath() const {
        return SHADERS_PATH;
    }

}