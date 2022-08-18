//
// Created by mecha on 17.11.2021.
//

#pragma once

#include <graphics/core/io/ShaderFile.h>
#include <ecs/Entity.h>

#include <platform/graphics/Shader.h>
#include <platform/graphics/UniformBuffer.h>

namespace engine::shader {

    class BaseShader;

    struct ShaderScript {
        static uint16_t IDS;
        uint16_t id = IDS++;
        std::function<void(const BaseShader&, ecs::Registry&)> updateRegistry;
        std::function<void(const BaseShader&, const ecs::Entity&)> updateEntity;

        ShaderScript() = default;
        ShaderScript(const ShaderScript&) = default;

        bool operator==(const ShaderScript& other) const {
            return id == other.id;
        }
    };

    class BaseShader final : public Shader {

    public:
        BaseShader() : Shader() {}
        BaseShader(const std::initializer_list<ShaderScript> &scripts): scripts(scripts) {}
        BaseShader(const char* src) : Shader(src) {}
        BaseShader(const char* src, const std::initializer_list<ShaderScript> &scripts)
        : Shader(src), scripts(scripts) {}
        ~BaseShader() = default;

    public:
        [[nodiscard]] inline const std::string& getSrc() const {
            return src;
        }

        inline void setUniformBlockFormat(const UniformBlockFormat& uniformBlockFormat) {
            uniformBuffer.setUniformBlockFormat(uniformBlockFormat);
        }

        inline void bindUniformBlockFormat() {
            const auto& uniformBlockFormat = uniformBuffer.getUniformBlockFormat();
            bindUbf(uniformBlockFormat.getName().data(), uniformBlockFormat.getId());
        }

        void applyUbf(const UniformBlockFormat& uniformBlockFormat);

        void updateUniformBuffer(const UniformData &uniformData) const;
        void updateUniformBuffer(IntUniform &uniform, const uint32_t &index = 0) const;
        void updateUniformBuffer(FloatUniform &uniform, const uint32_t &index = 0) const;
        void updateUniformBuffer(BoolUniform &uniform, const uint32_t &index = 0) const;
        void updateUniformBuffer(Vec3fUniform &uniform, const uint32_t &index = 0) const;
        void updateUniformBuffer(Vec4fUniform &uniform, const uint32_t &index = 0) const;
        void updateUniformBuffer(Mat4fUniform &uniform, const uint32_t &index = 0) const;
        void updateUniformBuffer(GLMMat4fUniform &uniform, const uint32_t &index = 0) const;

        void updateScripts(const ecs::Entity &entity) const;
        void updateScripts(ecs::Registry &registry) const;
        void addScript(const ShaderScript& script);
        void removeScript(const ShaderScript& script);
        void clearScripts();
        void release();

    protected:
        std::vector<ShaderScript> scripts;
        UniformBuffer uniformBuffer;

    };

    class BaseShaderProgram final : public ShaderProgram {

    public:
        BaseShaderProgram() = default;

        BaseShaderProgram(const io::ShaderProps &props) {
            construct(props);
        }

        BaseShaderProgram(
                const io::ShaderProps &props,
                const BaseShader &vShader
        ) : _vShader(vShader) {
            construct(props);
        }

        BaseShaderProgram(
                const io::ShaderProps &props,
                const BaseShader &vShader,
                const BaseShader &fShader
        ) : _vShader(vShader), _fShader(fShader) {
            construct(props);
        }

        BaseShaderProgram(
                const io::ShaderProps &props,
                const BaseShader &vShader,
                const BaseShader &fShader,
                const BaseShader &gShader
        ) : _vShader(vShader), _fShader(fShader), _gShader(gShader) {
            construct(props);
        }

        ~BaseShaderProgram() = default;

    public:
        [[nodiscard]] inline const VertexFormat& getVertexFormat() const {
            return vertexFormat;
        }

        [[nodiscard]] inline const BaseShader& getVShader() const {
            return _vShader;
        }

        [[nodiscard]] inline const BaseShader& getFShader() const {
            return _fShader;
        }

        inline VertexFormat& getVertexFormat() {
            return vertexFormat;
        }

    public:
        void construct(const io::ShaderProps& props);
        void detachShaders();
        void releaseShaders();
        bool invalidate();
        void bindVertexFormat();
        void parseVertexFormat();
        void parseUniformBlockFormat(BaseShader& shader);
        void update(ecs::Registry& registry);
        void update(const ecs::Entity& entity);
        void release();
        void recompile(const std::string& name);
        bool isReady();

    private:
        static ElementCount toElementCount(const std::string &elementCountStr);

    private:
        BaseShader _vShader; // Vertex Shader
        BaseShader _fShader; // Fragment/Pixel Shader
        BaseShader _gShader; // Geometry shader
        VertexFormat vertexFormat;
        uint32_t uniformBlockSlots = 0;

    };

}
