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

        explicit GLShader(const std::string& filePath) : Shader(filePath) {
            onCreate();
        }

        ~GLShader() override = default;

    public:
        void bind() override;
        void unbind() override;

        void setUniform(const Uniform<float> &uniform) override;
        void setUniform(const Uniform<int> &uniform) override;
        void setUniform(const Uniform<double> &uniform) override;

        void setUniform(const Uniform<glm::fvec2> &uniform) override;
        void setUniform(const Uniform<glm::fvec3> &uniform) override;
        void setUniform(const Uniform<glm::fvec4> &uniform) override;

        void setUniform(const Uniform<glm::fmat2> &uniform) override;
        void setUniform(const Uniform<glm::fmat3> &uniform) override;
        void setUniform(const Uniform<glm::fmat4> &uniform) override;

    private:
        static GLenum toShaderType(const std::string& type);
        static std::string toStringShaderType(const GLenum& type);
        static bool isTypeValid(const std::string& type);

    private:
        void onCreate();
        void onCreate(const std::string& vertexSrc, const std::string& fragmentSrc);
        void getTypeSources(const std::string& sources);
        void compile();

    private:
        std::unordered_map<GLenum, std::string> _typeSources;

    };

}