//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../../graphics/shader/Shader.h"
#include "../../../core/Assert.h"

#include "unordered_map"

#include "glad/glad.h"

namespace engine {

    class GLShader : public Shader {

    public:
        GLShader(const std::string& name,
                 const std::string& vertexSrc,
                 const std::string& fragmentSrc) : Shader(name) {
            onCreate(vertexSrc, fragmentSrc);
        }

        GLShader(const std::string& filePath) : Shader(filePath) {
            onCreate();
        }

        ~GLShader() override = default;

    public:
        void bind() override;
        void unbind() override;

        void setUniform(const char* name, const float &value) override;
        void setUniform(const char* name, const int &value) override;
        void setUniform(const char* name, const double &value) override;

        void setUniform(const char* name, const glm::fvec2 &value) override;
        void setUniform(const char* name, const glm::fvec3 &value) override;
        void setUniform(const char* name, const glm::fvec4 &value) override;

        void setUniform(const char* name, const glm::fmat2 &value) override;
        void setUniform(const char* name, const glm::fmat3 &value) override;
        void setUniform(const char* name, const glm::fmat4 &value) override;

        void prepare() override;

    private:
        static GLenum toShaderType(const std::string& type);
        static std::string toStringShaderType(const GLenum& type);
        static bool isTypeValid(const std::string& type);

    private:
        void onCreate(const std::string& vertexSrc, const std::string& fragmentSrc);
        void onCreate();
        void getTypeSources(const std::string& sources);
        void compile();

    private:
        std::unordered_map<GLenum, std::string> _typeSources;

    };

}