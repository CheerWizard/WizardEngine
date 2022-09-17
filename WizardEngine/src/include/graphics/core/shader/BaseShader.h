//
// Created by mecha on 17.11.2021.
//

#pragma once

#include <graphics/core/io/ShaderFile.h>
#include <ecs/Entity.h>

#include <platform/graphics/Shader.h>
#include <platform/graphics/UniformBuffer.h>

namespace engine::shader {

    class BaseShaderProgram;

    struct ENGINE_API ShaderScript {
        std::function<void(const BaseShaderProgram&, ecs::Registry&)> updateRegistry;
        std::function<void(const BaseShaderProgram&, const ecs::Entity&)> updateEntity;

        ShaderScript() = default;
        ShaderScript(const ShaderScript&) = default;
    };

    class ENGINE_API BaseShader final : public Shader {

    public:
        BaseShader() : Shader() {}
        BaseShader(const char* src) : Shader(src) {}
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

        void initUbf();
        void applyUbf(const UniformBlockFormat& uniformBlockFormat);

        void updateUniformBuffer(const UniformData &uniformData) const;
        void updateUniformBuffer(IntUniform &uniform, const uint32_t &index = 0) const;
        void updateUniformBuffer(FloatUniform &uniform, const uint32_t &index = 0) const;
        void updateUniformBuffer(BoolUniform &uniform, const uint32_t &index = 0) const;
        void updateUniformBuffer(Vec3fUniform &uniform, const uint32_t &index = 0) const;
        void updateUniformBuffer(Vec4fUniform &uniform, const uint32_t &index = 0) const;
        void updateUniformBuffer(Mat4fUniform &uniform, const uint32_t &index = 0) const;
        void updateUniformBuffer(GLMMat4fUniform &uniform, const uint32_t &index = 0) const;

        void release();

    protected:
        UniformBuffer uniformBuffer;

    };

    class ENGINE_API BaseShaderProgram final : public ShaderProgram {

    public:
        BaseShaderProgram() = default;

        BaseShaderProgram(
                const io::ShaderProps &props,
                const std::initializer_list<ShaderScript>& scripts = {}
        ) : scripts(scripts) {
            construct(props);
        }

        BaseShaderProgram(
                const io::ShaderProps &props,
                const BaseShader &vShader,
                const std::initializer_list<ShaderScript>& scripts = {}
        ) : _vShader(vShader), scripts(scripts) {
            construct(props);
        }

        BaseShaderProgram(
                const io::ShaderProps &props,
                const BaseShader &vShader,
                const BaseShader &fShader,
                const std::initializer_list<ShaderScript>& scripts = {}
        ) : _vShader(vShader), _fShader(fShader), scripts(scripts) {
            construct(props);
        }

        BaseShaderProgram(
                const io::ShaderProps &props,
                const BaseShader &vShader,
                const BaseShader &fShader,
                const BaseShader &gShader,
                const std::initializer_list<ShaderScript>& scripts = {}
        ) : _vShader(vShader), _fShader(fShader), _gShader(gShader), scripts(scripts) {
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

        inline void setInstancesPerDraw(u32 instancesPerDraw) {
            this->instancesPerDraw = instancesPerDraw;
        }

        [[nodiscard]] inline u32 getInstancesPerDraw() const {
            return instancesPerDraw;
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

        void addScript(const ShaderScript& script);
        void clearScripts();

    private:
        static ElementCount toElementCount(const std::string &elementCountStr);

    private:
        BaseShader _vShader; // Vertex Shader
        BaseShader _fShader; // Fragment/Pixel Shader
        BaseShader _gShader; // Geometry shader
        VertexFormat vertexFormat;
        uint32_t uniformBlockSlots = 0;
        u32 instancesPerDraw = 128;
        vector<ShaderScript> scripts;
    };
}
