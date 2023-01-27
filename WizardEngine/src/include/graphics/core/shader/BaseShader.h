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
        BaseShader(u32 shaderType) : Shader(shaderType) {}
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

        [[nodiscard]] inline const bool valid() const {
            return type != ShaderType::NONE;
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

    private:
        UniformBuffer uniformBuffer;
    };

    class ENGINE_API BaseShaderProgram final : public ShaderProgram {

    public:
        BaseShaderProgram() = default;

        BaseShaderProgram(
                const std::string& vFilepath,
                const std::string& fFilepath,
                const std::initializer_list<ShaderScript>& scripts = {}
        );

        BaseShaderProgram(
                const std::string& vFilepath,
                const std::string& fFilepath,
                const std::string& gFilepath,
                const std::initializer_list<ShaderScript>& scripts = {}
        );

        BaseShaderProgram(
                const std::string& vFilepath,
                const std::string& fFilepath,
                const std::string& gFilepath,
                const std::string& cFilepath,
                const std::initializer_list<ShaderScript>& scripts = {}
        );

        BaseShaderProgram(
                const std::string& vFilepath,
                const std::string& fFilepath,
                const std::string& gFilepath,
                const std::string& cFilepath,
                const std::string& tcFilepath,
                const std::string& teFilepath,
                const std::initializer_list<ShaderScript>& scripts = {}
        );

        ~BaseShaderProgram() = default;

    public:
        [[nodiscard]] inline const VertexFormat& getVertexFormat() const {
            return m_VertexFormat;
        }

        [[nodiscard]] inline const BaseShader& getVertexShader() const {
            return m_VertexShader;
        }

        [[nodiscard]] inline const BaseShader& getFragmentShader() const {
            return m_FragmentShader;
        }

        [[nodiscard]] inline BaseShader& getGeometryShader() {
            return m_GeometryShader;
        }

        [[nodiscard]] inline BaseShader& getComputeShader() {
            return m_ComputeShader;
        }

        [[nodiscard]] inline BaseShader& getTessControlShader() {
            return m_TessControlShader;
        }

        [[nodiscard]] inline BaseShader& getTessEvalShader() {
            return m_TessEvalShader;
        }

        inline VertexFormat& getVertexFormat() {
            return m_VertexFormat;
        }

        inline void setInstancesPerDraw(u32 instancesPerDraw) {
            m_InstancesPerDraw = instancesPerDraw;
        }

        [[nodiscard]] inline u32 getInstancesPerDraw() const {
            return m_InstancesPerDraw;
        }

    public:
        void detachShaders();
        void releaseShaders();
        bool invalidate();
        void bindVertexFormat();
        void parseVertexFormat();
        void parseUniformBlockFormat(BaseShader& shader);
        void update(ecs::Registry& registry);
        void update(const ecs::Entity& entity);
        void release();
        bool isReady();

        void addScript(const ShaderScript& script);
        void clearScripts();

    private:
        static ElementCount toElementCount(const std::string &elementCountStr);
        void createRequiredShader(const std::string& filepath, BaseShader& shader, u32 shaderType);
        void createOptionalShader(const std::string& filepath, BaseShader& shader, u32 shaderType);

    private:
        BaseShader m_VertexShader;
        BaseShader m_FragmentShader;
        BaseShader m_GeometryShader;
        BaseShader m_ComputeShader;
        BaseShader m_TessControlShader;
        BaseShader m_TessEvalShader;
        VertexFormat m_VertexFormat;
        uint32_t m_UniformBlocks = 0;
        u32 m_InstancesPerDraw = 128;
        vector<ShaderScript> m_Scripts;
    };
}
