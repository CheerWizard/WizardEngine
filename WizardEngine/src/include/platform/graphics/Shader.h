//
// Created by mecha on 05.09.2021.
//

#pragma once

#include <graphics/core/shader/Uniform.h>
#include <graphics/core/shader/VertexFormat.h>
#include <graphics/core/shader/UniformBlockFormat.h>

#include <core/primitives.h>
#include <core/vector.h>
#include <core/map.h>

namespace engine::shader {

    using namespace core;

    typedef s32 GLint;
    typedef u32 GLenum;

    enum ShaderState : u8 {
        READY = 0,
        FAILED_READ_FILE = 1,
        FAILED_COMPILE = 2,
        FAILED_LINKING = 3,
        NO_VERTEX_ATTRS = 4,
        NO_UNIFORM_BLOCKS = 5,
        NOT_READY = 6
    };

    class Shader {

    public:
        Shader() = default;
        Shader(const char* src) : src(src) {}
        ~Shader() = default;

    public:
        inline void setProgramId(const u32 &programId) {
            this->programId = programId;
        }

        inline void setSrc(const char* src) {
            this->src = src;
        }

        [[nodiscard]] inline bool hasSrc() const {
            return !src.empty();
        }

    public:
        void createVShader();
        void createFShader();
        void createGShader();
        void attach() const;
        void detach() const;
        void destroy() const;
        [[nodiscard]] bool compile() const;

        void bindUbf(const char* blockName, const u32& blockIndex) const;

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

        void setUniformArrayElement(const u32 &index, Mat4fUniform &uniform) const;
        void setUniformArrayElement(const u32 &index, IntUniform &uniform) const;
        void setUniformArrayElement(const u32 &index, FloatUniform &uniform) const;
        void setUniformArrayElement(const u32 &index, Vec4fUniform &uniform) const;

        void setUniformStructField(const char *structName, BoolUniform &structField) const;
        void setUniformStructField(const char *structName, IntUniform &structField) const;
        void setUniformStructField(const char *structName, FloatUniform &structField) const;
        void setUniformStructField(const char *structName, Vec3fUniform &structField) const;
        void setUniformStructField(const char *structName, Vec4fUniform &structField) const;

        void setUniformArrayStructField(
                const u32 &index,
                const char *structName,
                BoolUniform &structField
        ) const;
        void setUniformArrayStructField(
                const u32 &index,
                const char *structName,
                IntUniform &structField
        ) const;
        void setUniformArrayStructField(
                const u32 &index,
                const char *structName,
                FloatUniform &structField
        ) const;
        void setUniformArrayStructField(
                const u32 &index,
                const char *structName,
                Vec3fUniform &structField
        ) const;
        void setUniformArrayStructField(
                const u32 &index,
                const char *structName,
                Vec4fUniform &structField
        ) const;

    private:
        GLint getUniformArrayElementLocation(const char* name, const u32 &index) const;
        GLint getUniformArrayStructLocation(const char* structName, const char* fieldName, const u32 &index) const;
        GLint getUniformStructLocation(const char* structName, const char* fieldName) const;

    protected:
        u32 id = 0;
        u32 programId = 0;
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
        u32 bindAttribute(const char* attrName) const;

    protected:
        std::string toStringShaderType(GLenum type);

    protected:
        u32 id = 0;
        ShaderState state = ShaderState::FAILED_READ_FILE;

    };

}