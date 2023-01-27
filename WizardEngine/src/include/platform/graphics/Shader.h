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
    using namespace math;

    enum ShaderState : u8 {
        READY = 0,
        FAILED_READ_FILE = 1,
        FAILED_COMPILE = 2,
        FAILED_LINKING = 3,
        NO_VERTEX_ATTRS = 4,
        NO_UNIFORM_BLOCKS = 5,
        NOT_READY = 6
    };

    struct ENGINE_API ShaderType final {
        static u32 NONE;
        static u32 VERTEX;
        static u32 FRAGMENT;
        static u32 GEOMETRY;
        static u32 COMPUTE;
        static u32 TESS_CONTROL;
        static u32 TESS_EVAL;
    };

    class ENGINE_API Shader {

    public:
        Shader() = default;
        Shader(u32 shaderType);
        ~Shader() = default;

    public:
        inline void setProgramId(u32 programId) {
            this->programId = programId;
        }

        inline void setSrc(const char* src) {
            this->src = src;
        }

        [[nodiscard]] inline bool hasSrc() const {
            return !src.empty();
        }

    public:
        void attach() const;
        void detach() const;
        void destroy() const;
        [[nodiscard]] bool compile() const;

        void bindUbf(const char* blockName, u32 blockIndex) const;

    protected:
        u32 id = 0;
        u32 programId = 0;
        u32 type = 0;
        std::string src;
    };

    class ENGINE_API ShaderProgram {

    public:
        ShaderProgram() = default;
        ~ShaderProgram() = default;

    public:
        void create();
        void destroy() const;
        bool link();
        void start() const;
        static void stop() ;
        u32 bindAttribute(const char* attrName) const;

        void setUniform(const FloatUniform &uniform) const;
        void setUniform(const BoolUniform &uniform) const;
        void setUniform(const IntUniform &uniform) const;
        void setUniform(const DoubleUniform &uniform) const;
        void setUniform(Vec2fUniform &uniform) const;
        void setUniform(Vec3fUniform &uniform) const;
        void setUniform(GLMVec3fUniform &uniform) const;
        void setUniform(Vec4fUniform &uniform) const;
        void setUniform(Mat2fUniform &uniform) const;
        void setUniform(Mat3fUniform &uniform) const;
        void setUniform(Mat4fUniform &uniform) const;
        void setUniform(GLMMat4fUniform &uniform) const;

        template<typename T>
        void setUniformArray(const UniformArray<T> &uniformArray) const {
            for (u32 i = 0 ; i < uniformArray.values.size(); i++) {
                setUniformArrayElement(i, uniformArray.name, uniformArray.values[i]);
            }
        }

        void setUniformArrayElement(const u32 &index, const IntUniform &uniform) const;
        void setUniformArrayElement(const u32 &index, const FloatUniform &uniform) const;
        void setUniformArrayElement(const u32 &index, const BoolUniform &uniform) const;
        void setUniformArrayElement(const u32 &index, const DoubleUniform &uniform) const;

        void setUniformArrayElement(const u32 &index, Vec2fUniform &uniform) const;
        void setUniformArrayElement(const u32 &index, Vec3fUniform &uniform) const;
        void setUniformArrayElement(const u32 &index, Vec4fUniform &uniform) const;

        void setUniformArrayElement(const u32 &index, Mat2fUniform &uniform) const;
        void setUniformArrayElement(const u32 &index, Mat3fUniform &uniform) const;
        void setUniformArrayElement(const u32 &index, Mat4fUniform &uniform) const;
        void setUniformArrayElement(const u32 &index, GLMMat4fUniform &uniform) const;

        void setUniformArrayElement(const u32 &index, const char* arrayName, int v) const;
        void setUniformArrayElement(const u32 &index, const char* arrayName, float v) const;
        void setUniformArrayElement(const u32 &index, const char* arrayName, bool v) const;
        void setUniformArrayElement(const u32 &index, const char* arrayName, double v) const;

        void setUniformArrayElement(const u32 &index, const char* arrayName, vec2f& v) const;
        void setUniformArrayElement(const u32 &index, const char* arrayName, vec3f& v) const;
        void setUniformArrayElement(const u32 &index, const char* arrayName, vec4f& v) const;

        void setUniformArrayElement(const u32 &index, const char* arrayName, mat2f& v) const;
        void setUniformArrayElement(const u32 &index, const char* arrayName, mat3f& v) const;
        void setUniformArrayElement(const u32 &index, const char* arrayName, mat4f& v) const;
        void setUniformArrayElement(const u32 &index, const char* arrayName, glm::mat4& v) const;

        void setUniformStructField(const char *structName, const BoolUniform &structField) const;
        void setUniformStructField(const char *structName, const IntUniform &structField) const;
        void setUniformStructField(const char *structName, const FloatUniform &structField) const;
        void setUniformStructField(const char *structName, const DoubleUniform &structField) const;
        void setUniformStructField(const char *structName, Vec3fUniform &structField) const;
        void setUniformStructField(const char *structName, Vec4fUniform &structField) const;

        void setUniformArrayStructField(
                const u32 &index,
                const char *structName,
                const BoolUniform &structField
        ) const;
        void setUniformArrayStructField(
                const u32 &index,
                const char *structName,
                const IntUniform &structField
        ) const;
        void setUniformArrayStructField(
                const u32 &index,
                const char *structName,
                const FloatUniform &structField
        ) const;
        void setUniformArrayStructField(
                const u32 &index,
                const char *structName,
                const DoubleUniform &structField
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
        int getUniformArrayElementLocation(const char* name, u32 index) const;
        int getUniformArrayStructLocation(const char* structName, const char* fieldName, u32 index) const;
        int getUniformStructLocation(const char* structName, const char* fieldName) const;

    protected:
        u32 id = 0;
        ShaderState state = ShaderState::FAILED_READ_FILE;
    };

}