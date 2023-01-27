//
// Created by mecha on 17.11.2021.
//

#include <graphics/core/shader/BaseShader.h>

namespace engine::shader {

    BaseShaderProgram::BaseShaderProgram(
            const std::string &vFilepath,
            const std::string &fFilepath,
            const std::initializer_list<ShaderScript> &scripts) : m_Scripts(scripts) {
        createRequiredShader(vFilepath, m_VertexShader, ShaderType::VERTEX);
        createRequiredShader(fFilepath, m_FragmentShader, ShaderType::FRAGMENT);
        if (invalidate()) {
            parseVertexFormat();
            m_VertexShader.initUbf();
            m_FragmentShader.initUbf();
            parseUniformBlockFormat(m_VertexShader);
            parseUniformBlockFormat(m_FragmentShader);
            state = ShaderState::READY;
        }
    }

    BaseShaderProgram::BaseShaderProgram(
            const std::string &vFilepath,
            const std::string &fFilepath,
            const std::string &gFilepath,
            const std::initializer_list<ShaderScript> &scripts) : m_Scripts(scripts) {
        createRequiredShader(vFilepath, m_VertexShader, ShaderType::VERTEX);
        createRequiredShader(fFilepath, m_FragmentShader, ShaderType::FRAGMENT);
        createOptionalShader(gFilepath, m_GeometryShader, ShaderType::GEOMETRY);
        if (invalidate()) {
            parseVertexFormat();
            m_VertexShader.initUbf();
            m_FragmentShader.initUbf();
            parseUniformBlockFormat(m_VertexShader);
            parseUniformBlockFormat(m_FragmentShader);
            state = ShaderState::READY;
        }
    }

    BaseShaderProgram::BaseShaderProgram(
            const std::string &vFilepath,
            const std::string &fFilepath,
            const std::string &gFilepath,
            const std::string &cFilepath,
            const std::initializer_list<ShaderScript> &scripts) : m_Scripts(scripts) {
        createRequiredShader(vFilepath, m_VertexShader, ShaderType::VERTEX);
        createRequiredShader(fFilepath, m_FragmentShader, ShaderType::FRAGMENT);
        createOptionalShader(gFilepath, m_GeometryShader, ShaderType::GEOMETRY);
        createOptionalShader(cFilepath, m_ComputeShader, ShaderType::COMPUTE);
        if (invalidate()) {
            parseVertexFormat();
            m_VertexShader.initUbf();
            m_FragmentShader.initUbf();
            parseUniformBlockFormat(m_VertexShader);
            parseUniformBlockFormat(m_FragmentShader);
            state = ShaderState::READY;
        }
    }

    BaseShaderProgram::BaseShaderProgram(
            const std::string &vFilepath,
            const std::string &fFilepath,
            const std::string &gFilepath,
            const std::string &cFilepath,
            const std::string &tcFilepath,
            const std::string &teFilepath,
            const std::initializer_list<ShaderScript> &scripts) : m_Scripts(scripts) {
        createRequiredShader(vFilepath, m_VertexShader, ShaderType::VERTEX);
        createRequiredShader(fFilepath, m_FragmentShader, ShaderType::FRAGMENT);
        createOptionalShader(gFilepath, m_GeometryShader, ShaderType::GEOMETRY);
        createOptionalShader(cFilepath, m_ComputeShader, ShaderType::COMPUTE);
        createOptionalShader(tcFilepath, m_TessControlShader, ShaderType::TESS_CONTROL);
        createOptionalShader(teFilepath, m_TessEvalShader, ShaderType::TESS_EVAL);
        if (invalidate()) {
            parseVertexFormat();
            m_VertexShader.initUbf();
            m_FragmentShader.initUbf();
            parseUniformBlockFormat(m_VertexShader);
            parseUniformBlockFormat(m_FragmentShader);
            state = ShaderState::READY;
        }
    }

    void BaseShader::updateUniformBuffer(const UniformData &uniformData) const {
        uniformBuffer.bindBlock();
        uniformBuffer.bind();
        uniformBuffer.load(uniformData);
        uniformBuffer.unbind();
    }

    void BaseShader::updateUniformBuffer(IntUniform &uniform, const uint32_t &index) const {
        auto v = (float) uniform.value;
        updateUniformBuffer(UniformData { &v, index });
    }

    void BaseShader::updateUniformBuffer(BoolUniform &uniform, const uint32_t &index) const {
        auto intUniform = IntUniform {
            uniform.name,
            uniform.value ? 1 : 0
        };
        updateUniformBuffer(intUniform, index);
    }

    void BaseShader::updateUniformBuffer(FloatUniform &uniform, const uint32_t &index) const {
        auto& v = uniform.value;
        updateUniformBuffer(UniformData { &v, index });
    }

    void BaseShader::updateUniformBuffer(Vec3fUniform &uniform, const uint32_t &index) const {
        updateUniformBuffer(UniformData { toFloatPtr(uniform), index });
    }

    void BaseShader::updateUniformBuffer(Vec4fUniform &uniform, const uint32_t &index) const {
        updateUniformBuffer(UniformData { toFloatPtr(uniform), index });
    }

    void BaseShader::updateUniformBuffer(Mat4fUniform &uniform, const uint32_t &index) const {
        updateUniformBuffer(UniformData { toFloatPtr(uniform), index });
    }

    void BaseShader::updateUniformBuffer(GLMMat4fUniform &uniform, const uint32_t &index) const {
        updateUniformBuffer(UniformData { glm_toFloatPtr(uniform), index });
    }

    void BaseShader::applyUbf(const UniformBlockFormat &uniformBlockFormat) {
        uniformBuffer.applyFormat(uniformBlockFormat);
    }

    void BaseShader::release() {
        uniformBuffer.destroy();
        destroy();
    }

    void BaseShader::initUbf() {
        uniformBuffer.create();
    }

    void BaseShaderProgram::detachShaders() {
        m_VertexShader.detach();
        m_FragmentShader.detach();
    }

    void BaseShaderProgram::releaseShaders() {
        m_VertexShader.release();
        m_FragmentShader.release();
    }

    bool BaseShaderProgram::invalidate() {
        create();

        m_VertexShader.setProgramId(id);
        m_FragmentShader.setProgramId(id);
        m_GeometryShader.setProgramId(id);
        m_ComputeShader.setProgramId(id);

        // required shader, if compilation failed - skip invalidation
        if (m_VertexShader.valid() && m_VertexShader.compile()) {
            m_VertexShader.attach();
        } else {
            ENGINE_ERR("Vertex shader compilation failure!");
            state = FAILED_COMPILE;
            return false;
        }

        // required shader, if compilation failed - skip invalidation
        if (m_FragmentShader.valid() && m_FragmentShader.compile()) {
            m_FragmentShader.attach();
        } else {
            ENGINE_ERR("Fragment shader compilation failure!");
            state = FAILED_COMPILE;
            return false;
        }

        // optional shader, if compilation failed - continue invalidation
        if (m_GeometryShader.valid() && m_GeometryShader.compile()) {
            m_GeometryShader.attach();
        } else {
            ENGINE_WARN("Geometry shader compilation failure!");
        }

        // optional shader, if compilation failed - continue invalidation
        if (m_ComputeShader.valid() && m_ComputeShader.compile()) {
            m_ComputeShader.attach();
        } else {
            ENGINE_WARN("Compute shader compilation failure!");
        }

        if (!link()) {
            return false;
        }

        ENGINE_INFO("Shader program id={0} is ready for rendering!", id);
        return true;
    }

    void BaseShaderProgram::bindVertexFormat() {
        for (VertexAttribute& attribute : m_VertexFormat.getAttributes()) {
            attribute.location = bindAttribute(attribute.name.data());
        }
    }

    ElementCount BaseShaderProgram::toElementCount(const std::string &elementCountStr) {
        ElementCount elementCount = NONE;

        SWITCH(elementCountStr.c_str()) {
            CASE("float"):
                elementCount = SINGLE;
                break;
            CASE("int"):
                elementCount = SINGLE;
                break;
            CASE("bool"):
                elementCount = SINGLE;
                break;
            CASE("sampler2D"):
                elementCount = SINGLE;
                break;
            CASE("double"):
                elementCount = DOUBLE_;
                break;
            CASE("vec2"):
                elementCount = VEC2;
                break;
            CASE("dvec2"):
                elementCount = D_VEC2;
                break;
            CASE("vec3"):
                elementCount = VEC3;
                break;
            CASE("dvec3"):
                elementCount = D_VEC3;
                break;
            CASE("vec4"):
                elementCount = VEC4;
                break;
            CASE("dvec4"):
                elementCount = D_VEC4;
                break;
            CASE("mat2"):
                elementCount = MAT2_;
                break;
            CASE("mat3"):
                elementCount = MAT3;
                break;
            CASE("mat4"):
                elementCount = MAT4;
                break;
        }

        return elementCount;
    }

    void BaseShaderProgram::parseVertexFormat() {
        ENGINE_INFO("Shader is trying to find attributes...");
        std::string copy = m_VertexShader.getSrc();
        auto vShaderTokens = string::split(copy, " ");

        m_VertexFormat = VertexFormat();
        for (auto i = 0 ; i < vShaderTokens.size() ; i++) {
            if (vShaderTokens[i] == "in") {
                auto attrCategory = AttributeCategory::VERTEX;
                // find attr category token, if exists.
                if (vShaderTokens[i + 3] == "//") {
                    auto subToken = string::split(vShaderTokens[i + 4], "!");
                    if (!subToken.empty() && subToken[0] == "instance") {
                        attrCategory = AttributeCategory::INSTANCE;
                    }
                }

                auto attrElementCount = toElementCount(vShaderTokens[i + 1]);
                auto tokenName = vShaderTokens[i + 2];
                auto endNamePos = tokenName.find(';');
                auto attrName = tokenName.substr(0, endNamePos);
                auto attr = VertexAttribute {
                        attrName,
                        0,
                        0,
                        attrElementCount,
                        AttributeBool::FALSE,
                        attrCategory
                };
                ENGINE_INFO("Adding new vertex attribute - elementCount : {0}, name : {1}", attrElementCount, attrName);
                m_VertexFormat.add(attr);
            }
        }

        if (m_VertexFormat.isEmpty()) {
            ENGINE_WARN("Vertex Shader doesn't has vertex attributes!");
            state = NO_VERTEX_ATTRS;
        }

        ENGINE_INFO("Shader has found vertex attributes!");
    }

    void BaseShaderProgram::parseUniformBlockFormat(BaseShader& shader) {
//        auto shaderTypeStr = toStringShaderType(shaderType);
//        ENGINE_INFO("{0} shader is trying to find uniform structs and blocks...", shaderTypeStr);
        std::string copy = shader.getSrc();
        auto shaderTokens = string::split(copy, "\r\n; ");

        auto uniformBlockFormat = UniformBlockFormat(m_UniformBlocks++);

        auto uniformStructs = std::unordered_map<std::string, UniformStructAttribute>();
        // find all structs and uniform blocks!
        for (auto i = 0; i < shaderTokens.size(); i++) {
            // find struct by token
            if (shaderTokens[i] == "struct") {
                auto uniformStructName = shaderTokens[i + 1];
                auto uniformStruct = UniformStructAttribute { uniformStructName };
                ENGINE_INFO("Found struct {0} !", uniformStructName);
                // find all attrs in struct!
                auto j = i + 2;
                while (shaderTokens[j + 1] != "}") {
                    auto attrElementToken = shaderTokens[j + 1];
                    auto attrElementCount = toElementCount(attrElementToken);
                    if (attrElementCount == NONE) {
                        // check if struct exists in cache
                        if (uniformStructs.find(attrElementToken) != uniformStructs.end()) {
                            auto cachedUniformStruct = uniformStructs[attrElementToken];
                            uniformStruct.add(cachedUniformStruct.uniformAttributes);
                        } else {
                            ENGINE_WARN("{0} element is not recognized as struct!", attrElementToken);
                        }
                        // continue struct searching
                        j += 2;
                        continue;
                    }

                    auto attrName = shaderTokens[j + 2];
                    uint16_t attrCount = 1;
                    // check if this attr is attr array.
                    auto attrNameTokens = string::split(attrName, "[]");
                    if (attrNameTokens.size() > 1) {
                        attrName = attrNameTokens[0];
                        attrCount = TO_UINT16(attrNameTokens[1]);
                    }

                    auto attr = UniformAttribute {
                            attrName,
                            attrElementCount,
                            attrCount,
                            0
                    };

                    ENGINE_INFO("Adding uniform attribute {0} {1} to struct {2}", attrElementToken, attrName, uniformStructName);
                    uniformStruct.add(attr);

                    j += 2;
                }

                // cache uniform struct
                uniformStructs[uniformStructName] = uniformStruct;
            }

            if (shaderTokens[i] == "uniform") {
                // if uniform starts with curly brace, then this is uniform block
                if (shaderTokens[i + 2] != "{") continue;

                auto uniformBlockName = shaderTokens[i + 1];
                ENGINE_INFO("Found new uniform block - name : {0}", uniformBlockName);
                uniformBlockFormat.setName(uniformBlockName);

                auto j = i + 2; // start of block {
                while (shaderTokens[j + 1] != "}") {
                    auto attrElementToken = shaderTokens[j + 1];
                    auto attrElementCount = toElementCount(attrElementToken);
                    //check if element is struct
                    if (attrElementCount == NONE) {
                        // check if struct exists in cache
                        if (uniformStructs.find(attrElementToken) != uniformStructs.end()) {
                            auto uniformStruct = uniformStructs[attrElementToken];
                            uniformBlockFormat.add(uniformStruct.uniformAttributes);
                        } else {
                            ENGINE_WARN("{0} element is not recognized as struct!", attrElementToken);
                        }
                        // continue uniform attr searching
                        j += 2;
                        continue;
                    }

                    auto attrName = shaderTokens[j + 2];
                    uint16_t attrCount = 1;
                    // check if this attr is attr array.
                    auto attrNameTokens = string::split(attrName, "[]");
                    if (attrNameTokens.size() > 1) {
                        attrName = attrNameTokens[0];
                        attrCount = TO_UINT16(attrNameTokens[1]);
                    }

                    auto attr = UniformAttribute {
                            attrName,
                            attrElementCount,
                            attrCount,
                            0
                    };

                    uniformBlockFormat.add(attr);

                    j += 2;
                }
            }
            // if we found function main(), then we can stop searching, as all data should be declared before main() function!
            if (shaderTokens[i] == "void" && shaderTokens[i + 1] == "main()") {
                ENGINE_INFO("Found void main() function. Ending searching of uniform blocks!");
                break;
            }
        }

        if (uniformBlockFormat.isEmpty()) {
//            ENGINE_WARN("{0} shader {1} doesn't has uniform blocks!", shaderTypeStr, props.name);
            state = NO_UNIFORM_BLOCKS;
            return;
        }

        shader.bindUbf(uniformBlockFormat.getName().data(), uniformBlockFormat.getId());
        shader.applyUbf(uniformBlockFormat);
//        ENGINE_INFO("{0} shader has found uniform blocks!", shaderTypeStr);
    }

    void BaseShaderProgram::update(ecs::Registry& registry) {
        for (const auto& script : m_Scripts) {
            script.updateRegistry(*this, registry);
        }
    }

    void BaseShaderProgram::update(const ecs::Entity &entity) {
        for (const auto& script : m_Scripts) {
            script.updateEntity(*this, entity);
        }
    }

    void BaseShaderProgram::release() {
        clearScripts();
        detachShaders();
        releaseShaders();
        destroy();
        m_VertexFormat.clear();
    }

    bool BaseShaderProgram::isReady() {
        return state == ShaderState::READY;
    }

    void BaseShaderProgram::addScript(const ShaderScript& script) {
        m_Scripts.emplace_back(script);
    }

    void BaseShaderProgram::clearScripts() {
        m_Scripts.clear();
    }

    void BaseShaderProgram::createRequiredShader(const std::string& filepath, BaseShader& shader, u32 shaderType) {
        ENGINE_INFO("Create required shader {0}: ", filepath);
        const auto& src = io::ShaderFile::get().readShader(filepath);
        if (src.empty()) {
            state = FAILED_READ_FILE;
            return;
        }
        ENGINE_INFO("Shader {0}: \n {1}", filepath, src);
        shader = BaseShader(shaderType);
        shader.setSrc(src.c_str());
    }

    void BaseShaderProgram::createOptionalShader(const std::string& filepath, BaseShader& shader, u32 shaderType) {
        ENGINE_INFO("Create optional shader {0}: ", filepath);
        const auto& src = io::ShaderFile::get().readShader(filepath);
        if (src.empty()) {
            ENGINE_WARN("Shader failed to read from file : {0}", filepath);
            return;
        }
        ENGINE_INFO("Shader {0}: \n {1}", filepath, src);
        shader = BaseShader(shaderType);
        shader.setSrc(src.c_str());
    }
}