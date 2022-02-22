//
// Created by mecha on 17.11.2021.
//

#include <graphics/core/shader/BaseShader.h>

namespace engine::shader {

    uint16_t ShaderScript::IDS = 0;

    void BaseShaderProgram::construct(const ShaderProps &props) {
        auto sources = ShaderFile::readAssetWithIncludes(props);

        if (sources.vSrc.empty()) {
            state = FAILED_READ_FILE;
            return;
        }
        ENGINE_INFO("Vertex shader {0}: ", props.vFileName);
        ENGINE_INFO(sources.vSrc);

        if (sources.fSrc.empty()) {
            state = FAILED_READ_FILE;
            return;
        }
        ENGINE_INFO("Fragment shader {0}: ", props.fFileName);
        ENGINE_INFO(sources.fSrc);

        _vShader.setSrc(sources.vSrc.c_str());
        _fShader.setSrc(sources.fSrc.c_str());

        if (invalidate()) {
            parseVertexFormat();
            parseUniformBlockFormat(_vShader);
            parseUniformBlockFormat(_fShader);
            state = ShaderState::READY;
        }
    }

    void BaseShader::updateScripts(entt::registry &registry) const {
        for (const auto& script : scripts) {
            script.updateRegistry(*this, registry);
        }
    }

    void BaseShader::updateScripts(const Entity &entity) const {
        for (const auto& script : scripts) {
            script.updateEntity(*this, entity);
        }
    }

    void BaseShader::addScript(const ShaderScript& script) {
        scripts.emplace_back(script);
    }

    void BaseShader::removeScript(const ShaderScript& script) {
        scripts.erase(std::find(scripts.begin(), scripts.end(), script));
    }

    void BaseShader::clearScripts() {
        scripts.clear();
    }

    void BaseShader::updateUniformBuffer(const UniformData &uniformData) const {
        uniformBuffer.bindBlock();
        uniformBuffer.bind();
        uniformBuffer.load(uniformData);
        uniformBuffer.unbind();
    }

    void BaseShader::updateUniformBuffer(IntUniform &uniform, const uint32_t &index) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto v = (float) uniform.value;
        updateUniformBuffer(UniformData { &v, index });
    }

    void BaseShader::updateUniformBuffer(BoolUniform &uniform, const uint32_t &index) const {
        auto intUniform = IntUniform {
            uniform.name,
            uniform.value ? 1 : 0,
            uniform.isUpdated
        };
        updateUniformBuffer(intUniform, index);
    }

    void BaseShader::updateUniformBuffer(FloatUniform &uniform, const uint32_t &index) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto& v = uniform.value;
        updateUniformBuffer(UniformData { &v, index });
    }

    void BaseShader::updateUniformBuffer(Vec3fUniform &uniform, const uint32_t &index) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        updateUniformBuffer(UniformData { toFloatPtr(uniform), index });
    }

    void BaseShader::updateUniformBuffer(Vec4fUniform &uniform, const uint32_t &index) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        updateUniformBuffer(UniformData { toFloatPtr(uniform), index });
    }

    void BaseShader::updateUniformBuffer(Mat4fUniform &uniform, const uint32_t &index) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        updateUniformBuffer(UniformData { toFloatPtr(uniform), index });
    }

    void BaseShader::applyUbf(const UniformBlockFormat &uniformBlockFormat) {
        uniformBuffer.applyFormat(uniformBlockFormat);
    }

    void BaseShader::release() {
        clearScripts();
        uniformBuffer.destroy();
        destroy();
    }

    void BaseShaderProgram::detachShaders() {
        _vShader.detach();
        _fShader.detach();
    }

    void BaseShaderProgram::releaseShaders() {
        _vShader.release();
        _fShader.release();
    }

    bool BaseShaderProgram::invalidate() {
        create();
        _vShader.setProgramId(id);
        _fShader.setProgramId(id);

        _vShader.createVShader();
        if (!_vShader.compile()) {
            ENGINE_ERR("Shader compilation failure!");
            state = FAILED_COMPILE;
            return false;
        }
        _vShader.attach();

        _fShader.createFShader();
        if (!_fShader.compile()) {
            ENGINE_ERR("Shader compilation failure!");
            state = FAILED_COMPILE;
            return false;
        }
        _fShader.attach();

        if (!link()) {
            return false;
        }

        ENGINE_INFO("Shader program id={0} is ready for rendering!", id);
        return true;
    }

    void BaseShaderProgram::bindVertexFormat() {
        for (VertexAttribute& attribute : vertexFormat.getAttributes()) {
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
        std::string copy = _vShader.getSrc();
        auto vShaderTokens = split(copy, " ");

        vertexFormat = VertexFormat();
        for (auto i = 0 ; i < vShaderTokens.size() ; i++) {
            if (vShaderTokens[i] == "in") {
                auto attrCategory = VERTEX;
                // find attr category token, if exists.
                if (vShaderTokens[i + 3] == "//") {
                    auto subToken = split(vShaderTokens[i + 4], "!");
                    if (!subToken.empty() && subToken[0] == "instance") {
                        attrCategory = INSTANCE;
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
                        A_FALSE,
                        attrCategory
                };
                ENGINE_INFO("Adding new vertex attribute - elementCount : {0}, name : {1}", attrElementCount, attrName);
                vertexFormat.add(attr);
            }
        }

        if (vertexFormat.isEmpty()) {
            ENGINE_WARN("Vertex Shader doesn't has vertex attributes!");
            state = NO_VERTEX_ATTRS;
        }

        ENGINE_INFO("Shader has found vertex attributes!");
    }

    void BaseShaderProgram::parseUniformBlockFormat(BaseShader& shader) {
//        auto shaderTypeStr = toStringShaderType(shaderType);
//        ENGINE_INFO("{0} shader is trying to find uniform structs and blocks...", shaderTypeStr);
        std::string copy = shader.getSrc();
        auto shaderTokens = split(copy, "\r\n; ");

        auto uniformBlockFormat = UniformBlockFormat(uniformBlockSlots++);

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
                    auto attrNameTokens = split(attrName, "[]");
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
                    auto attrNameTokens = split(attrName, "[]");
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

    void BaseShaderProgram::update(entt::registry& registry) {
        _vShader.updateScripts(registry);
        _fShader.updateScripts(registry);
    }

    void BaseShaderProgram::update(const Entity &entity) {
        _vShader.updateScripts(entity);
        _fShader.updateScripts(entity);
    }

    void BaseShaderProgram::release() {
        detachShaders();
        releaseShaders();
        destroy();
        vertexFormat.clear();
    }

    void BaseShaderProgram::recompile(const std::string& name) {
        state = ShaderState::NOT_READY;
        release();
        construct(ShaderFile::getShaderProps(name));
    }

    bool BaseShaderProgram::isReady() {
        return state == ShaderState::READY;
    }

}