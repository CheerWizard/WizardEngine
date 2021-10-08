//
// Created by mecha on 05.09.2021.
//

#include "Shader.h"

namespace engine {

    const VertexAttribute& Shader::getAttribute(const uint32_t &index) const {
        return vertexFormat->get(index);
    }

    void Shader::removeAttribute(const uint32_t &index) {
        vertexFormat->remove(index);
    }

    std::string Shader::readShader(const std::string &name) {
        this->assetPath = props.assetPath;
        setAssetName(name);
        this->name = props.name;
        return read();
    }

    uint32_t Shader::addAttribute(const VertexAttribute &attribute) {
        return vertexFormat->add(attribute);
    }

    void Shader::replaceAttribute(const uint32_t &index, const VertexAttribute &attribute) {
        vertexFormat->replace(index, attribute);
    }

    const char *Shader::getAssetPath() const {
        return SHADERS_PATH;
    }

    uint32_t Shader::addUniformBlockAttr(const UniformAttribute &uniformAttribute) {
        return uniformBlockFormat->add(uniformAttribute);
    }

    const UniformAttribute& Shader::getUniformBlockAttr(const uint32_t &index) {
        return uniformBlockFormat->get(index);
    }

}