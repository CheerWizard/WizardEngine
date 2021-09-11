//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "glm/glm.hpp"

#include "Uniforms.h"

#include "../../core/File.h"

namespace engine {

    class Shader : public File {

    public:
        Shader(const std::string& name,
               const std::string& vertexSrc,
               const std::string& fragmentSrc) : name(name) {}

    public:
        explicit Shader(const std::string& filePath) : File(filePath) {}

    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void setUniform(const Uniform<float>& uniform) = 0;
        virtual void setUniform(const Uniform<int>& uniform) = 0;
        virtual void setUniform(const Uniform<double>& uniform) = 0;

        virtual void setUniform(const Uniform<glm::fvec2>& uniform) = 0;
        virtual void setUniform(const Uniform<glm::fvec3>& uniform) = 0;
        virtual void setUniform(const Uniform<glm::fvec4>& uniform) = 0;

        virtual void setUniform(const Uniform<glm::fmat2>& uniform) = 0;
        virtual void setUniform(const Uniform<glm::fmat3>& uniform) = 0;
        virtual void setUniform(const Uniform<glm::fmat4>& uniform) = 0;

    public:
        inline std::string& getName() {
            return name;
        }
        inline void setName(const std::string& name) {
            this->name = name;
        }

    protected:
        std::string name;
        uint32_t programId;

    };

}