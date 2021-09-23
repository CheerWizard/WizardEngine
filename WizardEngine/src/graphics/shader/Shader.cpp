//
// Created by mecha on 05.09.2021.
//

#include "Shader.h"

#include <sstream>

namespace engine {

    const Attribute& Shader::getAttribute(const uint32_t &index) const {
        return vertex->get(index);
    }

    void Shader::removeAttribute(const uint32_t &index) {
        vertex->remove(index);
    }

    std::string Shader::readShader(const std::string &path) const {
        std::stringstream srcPath;
        srcPath << SHADERS_PATH << path << getExtensionName();
        return read(srcPath.str());
    }

    uint32_t Shader::addAttribute(const Attribute &attribute) {
        return vertex->add(attribute);
    }

    void Shader::replaceAttribute(const uint32_t &index, const Attribute &attribute) {
        vertex->replace(index, attribute);
    }

}