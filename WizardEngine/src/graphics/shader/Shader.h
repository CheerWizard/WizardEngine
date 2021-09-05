//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../core/Memory.h"

#include "glm/glm.hpp"

namespace engine {

    class Shader {

    public:
        virtual ~Shader() = default;

    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void setInt(const std::string& name, int value) = 0;
        virtual void setIntArray(const std::string& name, int* values, uint32_t count) = 0;
        virtual void setFloat(const std::string& name, float value) = 0;
        virtual void setFloat2(const std::string& name, const glm::vec2& value) = 0;
        virtual void setFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void setFloat4(const std::string& name, const glm::vec4& value) = 0;
        virtual void setMat4(const std::string& name, const glm::mat4& value) = 0;

        virtual const std::string& getName() const = 0;
    };

}