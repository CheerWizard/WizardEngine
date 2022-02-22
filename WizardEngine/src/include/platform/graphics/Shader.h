//
// Created by mecha on 05.09.2021.
//

#pragma once

#include <graphics/core/shader/Uniform.h>
#include <graphics/core/shader/VertexFormat.h>
#include <graphics/core/shader/UniformBlockFormat.h>
#include <graphics/core/shader/ShaderData.h>

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace engine::shader {

    typedef int GLint;
    typedef unsigned int GLenum;

    class Shader {

    public:
        Shader() = default;
        Shader(const char* src) : src(src) {}
        ~Shader() = default;

    public:
        inline void setProgramId(const uint32_t &programId) {
            this->programId = programId;
        }

        inline void setSrc(const char* src) {
            this->src = src;
        }

    public:
        void createVShader();
        void createFShader();
        void attach() const;
        void detach() const;
        void destroy() const;
        [[nodiscard]] bool compile() const;

        void bindUbf(const char* blockName, const uint32_t& blockIndex) const;

        void setUniform(FloatUniform &uniform) const;
        void setUniform(BoolUniform &uniform) const;
        void setUniform(IntUniform &uniform) const;
        void setUniform(DoubleUniform &uniform) const;
        void setUniform(Vec2fUniform &uniform) const;
        void setUniform(Vec3fUniform &uniform) const;
        void setUniform(Vec4fUniform &uniform) const;
        void setUniform(Mat2fUniform &uniform) const;
        void setUniform(Mat3fUniform &uniform) const;
        void setUniform(Mat4fUniform &uniform) const;

        void setUniformArrayElement(const uint32_t &index, Mat4fUniform &uniform) const;
        void setUniformArrayElement(const uint32_t &index, IntUniform &uniform) const;
        void setUniformArrayElement(const uint32_t &index, FloatUniform &uniform) const;
        void setUniformStructField(const char *structName, BoolUniform &structField) const;
        void setUniformStructField(const char *structName, IntUniform &structField) const;
        void setUniformStructField(const char *structName, FloatUniform &structField) const;
        void setUniformStructField(const char *structName, Vec3fUniform &structField) const;
        void setUniformStructField(const char *structName, Vec4fUniform &structField) const;

        void setUniformArrayStructField(
                const uint32_t &index,
                const char *structName,
                BoolUniform &structField
        ) const;
        void setUniformArrayStructField(
                const uint32_t &index,
                const char *structName,
                IntUniform &structField
        ) const;
        void setUniformArrayStructField(
                const uint32_t &index,
                const char *structName,
                FloatUniform &structField
        ) const;
        void setUniformArrayStructField(
                const uint32_t &index,
                const char *structName,
                Vec3fUniform &structField
        ) const;
        void setUniformArrayStructField(
                const uint32_t &index,
                const char *structName,
                Vec4fUniform &structField
        ) const;

    private:
        GLint getUniformArrayElementLocation(const char* name, const uint32_t &index) const;
        GLint getUniformArrayStructLocation(const char* structName, const char* fieldName, const uint32_t &index) const;
        GLint getUniformStructLocation(const char* structName, const char* fieldName) const;

    protected:
        uint32_t id = 0;
        uint32_t programId = 0;
        std::string src;

    };

    class ShaderProgram {

    public:
        ShaderProgram() = default;
        ~ShaderProgram() = default;

    public:
        void create();
        void destroy() const;
        bool link();
        void start() const;
        void stop();
        uint32_t bindAttribute(const char* attrName) const;

    protected:
        std::string toStringShaderType(GLenum type);

    protected:
        uint32_t id = 0;
        ShaderState state = ShaderState::FAILED_READ_FILE;

    };

}