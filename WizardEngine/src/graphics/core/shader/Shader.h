//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "VertexFormat.h"
#include "Uniform.h"
#include "UniformBlockFormat.h"
#include "../buffers/UniformBuffer.h"

#include "../../../core/File.h"

#include "glm/glm.hpp"

#ifdef GL
#define CLIENT_SHADERS_PATH "assets/shaders/glsl/4.0"
#define ENGINE_SHADERS_PATH "../WizardEngine/assets/shaders/glsl/4.0"
#endif

#define DEFAULT_INCLUDE_TOKEN "#include"

namespace engine {

    enum ShaderState : unsigned char {
        READY = 0,
        FAILED_READ_FILE = 1,
        FAILED_COMPILE = 2,
        FAILED_LINKING = 3,
        NO_VERTEX_ATTRS = 4,
        NO_UNIFORM_BLOCKS = 5,
    };

    struct ShaderProps {
        std::string name;
        std::string vFileName;
        std::string fFileName;
        std::string assetPath = CLIENT_SHADERS_PATH;
        std::string includeToken = DEFAULT_INCLUDE_TOKEN;
    };

    class Shader : public File {

    public:
        Shader(const ShaderProps &shaderProps) : File(shaderProps.name, shaderProps.assetPath), props(shaderProps) {}

        Shader(const ShaderProps& shaderProps, VertexFormat* vertexFormat) :
        File(shaderProps.name, shaderProps.assetPath),
        vertexFormat(vertexFormat),
        props(shaderProps) {}

    public:
        inline void setVertexFormat(VertexFormat* vertexFormat) {
            this->vertexFormat = vertexFormat;
        }

        [[nodiscard]] inline VertexFormat* getVertexFormat() const {
            return vertexFormat;
        }

        inline void setProps(const ShaderProps& shaderProps) {
            props = shaderProps;
        }

        [[nodiscard]] inline const ShaderProps& getProps() const {
            return props;
        }

        [[nodiscard]] inline ShaderState getState() const {
            return state;
        }

        [[nodiscard]] inline const uint32_t& getId() const {
            return programId;
        }

        [[nodiscard]] inline UniformBlockFormat* getVertexUniformFormat() const {
            return vUniformBlockFormat;
        }

        inline void setVertexUniformFormat(UniformBlockFormat* uniformBlockFormat) {
            vUniformBlockFormat = uniformBlockFormat;
        }

        [[nodiscard]] inline UniformBlockFormat* getFragmentUniformFormat() const {
            return fUniformBlockFormat;
        }

        inline void setFragmentUniformFormat(UniformBlockFormat* uniformBlockFormat) {
            fUniformBlockFormat = uniformBlockFormat;
        }

        [[nodiscard]] inline bool hasVUniformBlocks() const {
            return !vUniformBlockFormat->isEmpty();

        }

        [[nodiscard]] inline bool hasFUniformBlocks() const {
            return !fUniformBlockFormat->isEmpty();
        }

    public:
        uint32_t addAttribute(const VertexAttribute &attribute);
        void replaceAttribute(const uint32_t &index, const VertexAttribute &attribute);
        const VertexAttribute& getAttribute(const uint32_t &index) const;
        void removeAttribute(const uint32_t &index);

        void updateVUniformBuffer(const UniformData &uniformData);
        void updateFUniformBuffer(const UniformData &uniformData);

        void bindVUniformBlock();
        void bindFUniformBlock();

    public:
        virtual void start() = 0;
        virtual void stop() = 0;

        virtual void bindAttributes() = 0;

        virtual void setUniform(FloatUniform &uniform) = 0;
        virtual void setUniform(BoolUniform &uniform) = 0;
        virtual void setUniform(IntUniform &uniform) = 0;
        virtual void setUniform(DoubleUniform &uniform) = 0;

        virtual void setUniform(Vec2fUniform &uniform) = 0;
        virtual void setUniform(Vec3fUniform &uniform) = 0;
        virtual void setUniform(Vec4fUniform &uniform) = 0;

        virtual void setUniform(Mat2fUniform &uniform) = 0;
        virtual void setUniform(Mat3fUniform &uniform) = 0;
        virtual void setUniform(Mat4fUniform &uniform) = 0;

        virtual void setUniformArrayElement(const uint32_t &index, Mat4fUniform &uniform) = 0;
        virtual void setUniformArrayElement(const uint32_t &index, IntUniform &uniform) = 0;
        virtual void setUniformArrayElement(const uint32_t &index, FloatUniform &uniform) = 0;

        virtual void setUniformStructField(const char* structName, FloatUniform &structField) = 0;
        virtual void setUniformStructField(const char* structName, Vec3fUniform &structField) = 0;
        virtual void setUniformStructField(const char* structName, Vec4fUniform &structField) = 0;

        virtual void setUniformArrayStructField(
                const uint32_t &index,
                const char* structName,
                FloatUniform &structField
        ) = 0;

        virtual void setUniformArrayStructField(
                const uint32_t &index,
                const char* structName,
                Vec3fUniform &structField
        ) = 0;

        virtual void setUniformArrayStructField(
                const uint32_t &index,
                const char* structName,
                Vec4fUniform &structField
        ) = 0;

    protected:
        void updateUniformBuffer(const Ref<UniformBuffer> &uniformBuffer, const UniformData &uniformData);
        virtual void bindUniformBlock(UniformBlockFormat* uniformBlockFormat) = 0;

    protected:
        uint32_t programId;
        ShaderProps props;
        ShaderState state = READY;
        VertexFormat* vertexFormat = nullptr;
        Ref<UniformBuffer> vUniformBuffer = nullptr;
        UniformBlockFormat* vUniformBlockFormat = nullptr;
        Ref<UniformBuffer> fUniformBuffer = nullptr;
        UniformBlockFormat* fUniformBlockFormat = nullptr;
    };

}