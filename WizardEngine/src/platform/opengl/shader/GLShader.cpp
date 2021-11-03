//
// Created by mecha on 05.09.2021.
//

#include "GLShader.h"

#include "../buffers/GLUniformBuffer.h"

namespace engine {

    void GLShader::onCreate() {
        auto vShader = readAsset(props.vFileName);
        if (vShader.empty()) {
            state = FAILED_READ_FILE;
            return;
        }

        auto fShader = readAsset(props.fFileName);
        if (fShader.empty()) {
            state = FAILED_READ_FILE;
            return;
        }

        _typeSources[GL_VERTEX_SHADER] = vShader;
        _typeSources[GL_FRAGMENT_SHADER] = fShader;

        if (compile()) {
            if (vertexFormat == nullptr) {
                findAttributes();
            }
            if (uniformBlockFormat == nullptr) {
                findUniformBlocks();
            }
        }
    }

    void GLShader::start() {
        glUseProgram(programId);
    }

    void GLShader::stop() {
        glUseProgram(0);
    }

    void GLShader::bindAttributes() {
        for (VertexAttribute& attribute : vertexFormat->getAttributes()) {
            auto attrName = attribute.name.c_str();
            auto attributeLocation = glGetAttribLocation(programId, attrName);
            glBindAttribLocation(programId, attributeLocation, attrName);
            attribute.location = attributeLocation;
        }
    }

    void GLShader::bindUniformBlock() {
        auto uniformBlockIndex = glGetUniformBlockIndex(programId, uniformBlockFormat->getName().c_str());
        glUniformBlockBinding(programId, uniformBlockIndex, 0);
    }

    void GLShader::setUniform(FloatUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniform1f(location, uniform.value);
    }

    void GLShader::setUniform(BoolUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniform1i(location, uniform.value); // bool is 32-bit as integer in GLSL compiler.
    }

    void GLShader::setUniform(IntUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniform1i(location, uniform.value);
    }

    void GLShader::setUniform(DoubleUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniform1d(location, uniform.value);
    }

    void GLShader::setUniform(Vec2fUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        auto value = uniform.value;
        glUniform2fv(location, 1, uniform.toFloatPtr());
    }

    void GLShader::setUniform(Vec3fUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        auto value = uniform.value;
        glUniform3fv(location, 1, uniform.toFloatPtr());
    }

    void GLShader::setUniform(Vec4fUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        auto value = uniform.value;
        glUniform4fv(location, 1, uniform.toFloatPtr());
    }

    void GLShader::setUniform(Mat2fUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniformMatrix2fv(location, 1, GL_FALSE, uniform.toFloatPtr());
    }

    void GLShader::setUniform(Mat3fUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniformMatrix3fv(location, 1, GL_FALSE, uniform.toFloatPtr());
    }

    void GLShader::setUniform(Mat4fUniform &uniform) {
        ENGINE_INFO("Mat4Uniform is updated : {0}", uniform.isUpdated);
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniformMatrix4fv(location, 1, GL_FALSE, uniform.toFloatPtr());
    }

    void GLShader::setUniformArrayElement(const uint32_t &index, Mat4fUniform &uniform) {
        ENGINE_INFO("Mat4Uniform array element {0} is updated: {1}", uniform.name, uniform.isUpdated);
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;

        auto name = std::string(uniform.name) + "[" + std::to_string(index) + "]";
        auto c_name = name.c_str();
        auto location = glGetUniformLocation(programId, c_name);

        ENGINE_INFO("Uploading Mat4Uniform array element into {0}", c_name);
        glUniformMatrix4fv(location, 1, GL_FALSE, uniform.toFloatPtr());
    }

    void GLShader::setUniformArrayElement(const uint32_t &index, IntUniform &uniform) {
        ENGINE_INFO("IntUniform array element {0} is updated: {1}", uniform.name, uniform.isUpdated);
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;

        auto name = std::string(uniform.name) + "[" + std::to_string(index) + "]";
        auto c_name = name.c_str();
        auto location = glGetUniformLocation(programId, c_name);

        ENGINE_INFO("Uploading IntUniform array element into {0}", c_name);
        glUniform1i(location, uniform.value);
    }

    bool GLShader::compile() {
        programId = glCreateProgram();
        _shaderIds = std::vector<GLenum>();

        for (auto& typeSource : _typeSources) {
            GLenum type = typeSource.first;
            const std::string& source = typeSource.second;

            GLuint shader = glCreateShader(type);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, nullptr);

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

            if (isCompiled == GL_FALSE) {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                ENGINE_ERR("{0}", infoLog.data());
                auto shaderStringType = toStringShaderType(type);
                ENGINE_ASSERT(false, shaderStringType + " shader compilation failure!");

                state = FAILED_COMPILE;
                return false;
            }

            glAttachShader(programId, shader);
            _shaderIds.push_back(shader);
        }

        glLinkProgram(programId);
        GLint isLinked = 0;
        glGetProgramiv(programId, GL_LINK_STATUS, (int*) &isLinked);

        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);

            ENGINE_ERR("{0}", infoLog.data());
            ENGINE_ASSERT(false, "Shader link failure!");

            state = FAILED_LINKING;
            return false;
        }

        ENGINE_INFO("Shader program id={0} has been fully compiled!", programId);
        return true;
    }

    std::string GLShader::toStringShaderType(GLenum type) {
        switch (type) {
            case GL_VERTEX_SHADER:
                return "Vertex";
            case GL_FRAGMENT_SHADER:
                return "Fragment";
        }

        ENGINE_ASSERT(false, "Cannot convert shader type to string!");
        return "Undefined";
    }

    void GLShader::onDestroy() {
        detachShaders();
        deleteShaders();
        glDeleteProgram(programId);
        _shaderIds.clear();
        _typeSources.clear();
        delete vertexFormat;
        delete uniformBlockFormat;
    }

    void GLShader::detachShaders() {
        for (auto shaderId : _shaderIds) {
            glDetachShader(programId, shaderId);
        }
    }

    void GLShader::deleteShaders() {
        for (auto shaderId : _shaderIds) {
            glDeleteShader(shaderId);
        }
    }

    const char *GLShader::getExtensionName() const {
        return ".glsl";
    }

    void GLShader::findAttributes() {
        auto vShader = _typeSources[GL_VERTEX_SHADER];
        ENGINE_INFO("Shader is trying to find attributes...");
        auto vShaderTokens = split(vShader, " ");

        vertexFormat = new VertexFormat();
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
                    attrElementCount,
                    0,
                    attrCategory
                };
                ENGINE_INFO("Adding new vertex attribute - elementCount : {0}, name : {1}", attrElementCount, attrName);
                addAttribute(attr);
            }
        }

        if (vertexFormat->isEmpty()) {
            ENGINE_WARN("Shader '{0}' doesn't has vertex attributes!", props.name);
            state = NO_VERTEX_ATTRS;
        }

        ENGINE_INFO("Shader has found vertex attributes!");
    }

    ElementCount GLShader::toElementCount(const std::string &elementCountStr) {
        ElementCount elementCount = SINGLE;

        SWITCH(elementCountStr.c_str()) {
            CASE("vec2"):
                elementCount = VEC2;
                break;
            CASE("vec3"):
                elementCount = VEC3;
                break;
            CASE("vec4"):
                elementCount = VEC4;
                break;
            CASE("mat2"):
                elementCount = MAT2;
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

    void GLShader::findUniformBlocks() {
        auto vShader = _typeSources[GL_VERTEX_SHADER];
        ENGINE_INFO("Shader is trying to find uniform blocks...");
        auto vShaderTokens = split(vShader, "\r\n; ");

        uniformBlockFormat = new UniformBlockFormat();
        for (auto i = 0 ; i < vShaderTokens.size() ; i++) {
            if (vShaderTokens[i] == "uniform") {
                // if uniform starts with curly brace, then this is uniform block
                if (vShaderTokens[i + 2] != "{") continue;

                auto uniformBlockName = vShaderTokens[i + 1];
                ENGINE_INFO("Found new uniform block - name : {0}", uniformBlockName);
                uniformBlockFormat->setName(uniformBlockName);

                auto j = i + 2; // start of block {
                while (vShaderTokens[j + 1] != "}") {
                    auto attrElementCount = toElementCount(vShaderTokens[j + 1]);

                    auto attrName = vShaderTokens[j + 2];
                    uint32_t attrCount = 1;
                    // check if this attr is attr array.
                    auto attrNameTokens = split(attrName, "[]");
                    if (attrNameTokens.size() > 1) {
                        attrName = attrNameTokens[0];
                        attrCount = TO_UINT32(attrNameTokens[1]);
                    }

                    auto attr = UniformAttribute {
                        attrName,
                        attrElementCount,
                        attrCount
                    };

                    ENGINE_INFO(
                        "Adding new uniform block attribute - elementCount : {0}, block name : {1}",
                        attrElementCount,
                        uniformBlockName
                    );
                    addUniformBlockAttr(attr);

                    j += 2;
                }

                // skip searching next blocks
                break;
            }
        }

        if (uniformBlockFormat->isEmpty()) {
            ENGINE_WARN("Shader '{0}' doesn't has uniform blocks!", props.name);
            state = NO_UNIFORM_BLOCKS;
            return;
        }

        uniformBuffer = createRef<GLUniformBuffer>();
        bindUniformBlock();
        uniformBuffer->prepare(getUniformBlockFormat());

        ENGINE_INFO("Shader has found uniform blocks!");
    }

}