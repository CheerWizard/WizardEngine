//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "glm/glm.hpp"

#include "../../core/File.h"

#include "../geometry/Vertex.h"

#include "../../math/Uniform.h"

#define SHADERS_PATH "assets/shaders/"

namespace engine {

    struct ShaderProps {
        std::string name;
        std::string vertexPath;
        std::string fragmentPath;

        ShaderProps(
                const std::string& name,
                const std::string& vertexPath,
                const std::string& fragmentPath) :
                name(name),
                vertexPath(vertexPath),
                fragmentPath(fragmentPath) {}
    };

    class Shader : public File {

    public:
        Shader(const ShaderProps& shaderProps, Vertex* vertex) :
        File(shaderProps.name, ""),
        vertex(vertex),
        props(shaderProps) {}

    public:
        inline void setVertex(Vertex* vertex) {
            this->vertex = vertex;
        }

        inline Vertex *getVertex() const {
            return vertex;
        }

        inline void setProps(const ShaderProps& shaderProps) {
            props = shaderProps;
        }

        inline const ShaderProps& getProps() const {
            return props;
        }

    public:
        uint32_t addAttribute(const Attribute &attribute);
        void replaceAttribute(const uint32_t &index, const Attribute &attribute);
        const Attribute& getAttribute(const uint32_t &index) const;
        void removeAttribute(const uint32_t &index);

    protected:
        std::string readShader(const std::string &path) const;

    public:
        virtual void start() = 0;
        virtual void stop() = 0;

        virtual void prepare() = 0;

        virtual void setUniform(const char* name, const float &value) = 0;
        virtual void setUniform(const char* name, const int &value) = 0;
        virtual void setUniform(const char* name, const double &value) = 0;

        virtual void setUniform(const char* name, const glm::fvec2 &value) = 0;
        virtual void setUniform(const char* name, const glm::fvec3 &value) = 0;
        virtual void setUniform(const char* name, const glm::fvec4 &value) = 0;

        virtual void setUniform(const char* name, const glm::fmat2 &value) = 0;
        virtual void setUniform(const char* name, const glm::fmat3 &value) = 0;

        virtual void setUniform(const Mat4fUniform &mat4Uniform) = 0;

    protected:
        virtual const char* getExtensionName() const = 0;

    protected:
        uint32_t programId;
        Vertex* vertex;
        ShaderProps props;

    };

}