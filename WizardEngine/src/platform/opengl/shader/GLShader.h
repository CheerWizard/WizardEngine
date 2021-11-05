//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "../../../graphics/core/shader/Shader.h"
#include "../../../core/Assert.h"

#include "unordered_map"

#include "glad/glad.h"

namespace engine {

    class GLShader : public Shader {

    public:
        GLShader(const ShaderProps &shaderProps) : Shader(shaderProps) {
            onCreate();
        }

        GLShader(const ShaderProps& shaderProps, VertexFormat* vertexFormat) : Shader(shaderProps, vertexFormat) {
            onCreate();
        }

        ~GLShader() override {
            onDestroy();
        }

    public:
        void start() override;
        void stop() override;

        void bindAttributes() override;

        void setUniform(FloatUniform &uniform) override;
        void setUniform(BoolUniform &uniform) override;
        void setUniform(IntUniform &uniform) override;
        void setUniform(DoubleUniform &uniform) override;

        void setUniform(Vec2fUniform &uniform) override;
        void setUniform(Vec3fUniform &uniform) override;
        void setUniform(Vec4fUniform &uniform) override;

        void setUniform(Mat2fUniform &uniform) override;
        void setUniform(Mat3fUniform &uniform) override;
        void setUniform(Mat4fUniform &uniform) override;

        void setUniformArrayElement(const uint32_t &index, Mat4fUniform &uniform) override;
        void setUniformArrayElement(const uint32_t &index, IntUniform &uniform) override;
        void setUniformArrayElement(const uint32_t &index, FloatUniform &uniform) override;

        void setUniformArrayStructField(
                const uint32_t &index,
                const char* structName,
                FloatUniform &uniform
        ) override;

        void setUniformArrayStructField(const uint32_t &index, const char *structName, Vec3fUniform &uniform) override;

        void
        setUniformArrayStructField(const uint32_t &index, const char *structName, Vec4fUniform &structField) override;

        void setUniformStructField(const char *structName, FloatUniform &structField) override;

        void setUniformStructField(const char *structName, Vec3fUniform &structField) override;

        void setUniformStructField(const char *structName, Vec4fUniform &structField) override;

    private:
        GLint getUniformArrayElementLocation(const char* name, const uint32_t &index);
        GLint getUniformArrayStructLocation(const char* structName, const char* fieldName, const uint32_t &index);
        GLint getUniformStructLocation(const char* structName, const char* fieldName);
        static std::string toStringShaderType(GLenum type);

    private:
        void onCreate();
        void onDestroy();
        bool compile();
        void detachShaders();
        void deleteShaders();
        void findAttributes();
        void findUniformBlocks(const GLenum &shaderType, UniformBlockFormat* uniformBlockFormat, Ref<UniformBuffer>& uniformBuffer);
        static ElementCount toElementCount(const std::string &elementCountStr);

    protected:
        const char* getExtensionName() const override;

        void bindUniformBlock(UniformBlockFormat *uniformBlockFormat) override;

    private:
        std::unordered_map<GLenum, std::string> _typeSources;
        std::vector<GLenum> _shaderIds;

    };

}