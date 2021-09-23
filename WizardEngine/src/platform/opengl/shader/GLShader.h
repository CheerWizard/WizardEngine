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
        GLShader(const ShaderProps& shaderProps, Vertex* vertex) : Shader(shaderProps, vertex) {
            onCreate();
        }

        ~GLShader() override {
            onDestroy();
        }

    public:
        void start() override;
        void stop() override;

        void setUniform(const char* name, const float &value) override;
        void setUniform(const char* name, const int &value) override;
        void setUniform(const char* name, const double &value) override;

        void setUniform(const char* name, const glm::fvec2 &value) override;
        void setUniform(const char* name, const glm::fvec3 &value) override;
        void setUniform(const char* name, const glm::fvec4 &value) override;

        void setUniform(const char* name, const glm::fmat2 &value) override;
        void setUniform(const char* name, const glm::fmat3 &value) override;

        void setUniform(const Mat4fUniform &mat4Uniform) override;

        void prepare() override;

    private:
        static std::string toStringShaderType(GLenum type);

    private:
        void onCreate();
        void onDestroy();
        void compile();
        void detachShaders();
        void deleteShaders();

    protected:
        const char* getExtensionName() const override;

    private:
        std::unordered_map<GLenum, std::string> _typeSources;
        std::vector<GLenum> _shaderIds;

    };

}