//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "glm/glm.hpp"

#include "../../core/File.h"

#include "../geometry/Vertex.h"

namespace engine {

    class Shader : public File {

    public:
        Shader(const std::string& name,
               const std::string& vertexSrc,
               const std::string& fragmentSrc) : File(name, "") {}

        Shader(const std::string& filePath) : File("", filePath) {}

    public:
        inline void setVertex(Vertex* vertex) {
            this->vertex = vertex;
        }

        inline Vertex *getVertex() const {
            return vertex;
        }

    public:
        uint32_t addAttribute(Attribute* attribute);
        void replaceAttribute(const uint32_t &index, Attribute* attribute);
        Attribute& getAttribute(const uint32_t &index) const;
        void removeAttribute(const uint32_t &index);

    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void prepare() = 0;

        virtual void setUniform(const char* name, const float &value) = 0;
        virtual void setUniform(const char* name, const int &value) = 0;
        virtual void setUniform(const char* name, const double &value) = 0;

        virtual void setUniform(const char* name, const glm::fvec2 &value) = 0;
        virtual void setUniform(const char* name, const glm::fvec3 &value) = 0;
        virtual void setUniform(const char* name, const glm::fvec4 &value) = 0;

        virtual void setUniform(const char* name, const glm::fmat2 &value) = 0;
        virtual void setUniform(const char* name, const glm::fmat3 &value) = 0;
        virtual void setUniform(const char* name, const glm::fmat4 &value) = 0;

    protected:
        uint32_t programId;
        Vertex* vertex;

    };

}