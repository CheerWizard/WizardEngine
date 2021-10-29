//
// Created by mecha on 05.09.2021.
//

#pragma once

#include "VertexFormat.h"
#include "Uniform.h"
#include "UniformBlockFormat.h"

#include "../../core/File.h"

#include "glm/glm.hpp"

#ifdef GL
#define CLIENT_SHADERS_PATH "assets/shaders/glsl/4.0"
#define ENGINE_SHADERS_PATH "../WizardEngine/assets/shaders/glsl/4.0"
#endif

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

        inline VertexFormat* getVertexFormat() const {
            return vertexFormat;
        }

        inline void setProps(const ShaderProps& shaderProps) {
            props = shaderProps;
        }

        inline const ShaderProps& getProps() const {
            return props;
        }

        inline ShaderState getState() const {
            return state;
        }

        inline const uint32_t& getId() const {
            return programId;
        }

        inline void setUniformFormat(UniformBlockFormat* uniformBlockFormat) {
            this->uniformBlockFormat = uniformBlockFormat;
        }

        inline UniformBlockFormat* getUniformBlockFormat() const {
            return uniformBlockFormat;
        }

    public:
        uint32_t addAttribute(const VertexAttribute &attribute);
        void replaceAttribute(const uint32_t &index, const VertexAttribute &attribute);
        const VertexAttribute& getAttribute(const uint32_t &index) const;
        void removeAttribute(const uint32_t &index);

        uint32_t addUniformBlockAttr(const UniformAttribute &uniformAttribute);
        const UniformAttribute& getUniformBlockAttr(const uint32_t &index);

    public:
        virtual void start() = 0;
        virtual void stop() = 0;

        virtual void bindAttributes() = 0;
        virtual void bindUniformBlock() = 0;

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

    protected:
        uint32_t programId;
        ShaderProps props;
        ShaderState state = READY;
        VertexFormat* vertexFormat = nullptr;
        UniformBlockFormat* uniformBlockFormat = nullptr;

    };

}