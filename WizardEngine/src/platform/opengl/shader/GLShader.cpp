//
// Created by mecha on 05.09.2021.
//

#include "GLShader.h"

#include "../buffers/GLUniformBuffer.h"

#include "sstream"

namespace engine {

    void GLShader::onCreate() {
        auto vShader = readAssetWithIncludes(props.vFileName, props.includeToken);
        if (vShader.empty()) {
            state = FAILED_READ_FILE;
            return;
        }
        ENGINE_INFO("Vertex shader {0}: ", props.vFileName);
        ENGINE_INFO(vShader);

        auto fShader = readAssetWithIncludes(props.fFileName, props.includeToken);
        if (fShader.empty()) {
            state = FAILED_READ_FILE;
            return;
        }
        ENGINE_INFO("Fragment shader {0}: ", props.fFileName);
        ENGINE_INFO(fShader);

        _typeSources[GL_VERTEX_SHADER] = vShader;
        _typeSources[GL_FRAGMENT_SHADER] = fShader;

        if (compile()) {
            if (vertexFormat == nullptr) {
                findAttributes();
            }

            if (vUniformBlockFormat == nullptr) {
                vUniformBlockFormat = new UniformBlockFormat();
                findUniformBlocks(GL_VERTEX_SHADER, vUniformBlockFormat, vUniformBuffer);
            }

            if (fUniformBlockFormat == nullptr) {
                fUniformBlockFormat = new UniformBlockFormat();
                findUniformBlocks(GL_FRAGMENT_SHADER, fUniformBlockFormat, fUniformBuffer);
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

    void GLShader::bindUniformBlock(UniformBlockFormat* uniformBlockFormat) {
        auto uniformBlockIndex = glGetUniformBlockIndex(programId, uniformBlockFormat->getName().c_str());
        glUniformBlockBinding(programId, uniformBlockIndex, 0);
    }

    GLint GLShader::getUniformArrayElementLocation(const char* name, const uint32_t &index) {
        std::stringstream ss;
        ss << name << "[" << std::to_string(index) << "]";
        return glGetUniformLocation(programId, ss.str().c_str());
    }

    GLint GLShader::getUniformArrayStructLocation(const char* structName, const char* fieldName, const uint32_t &index) {
        std::stringstream ss;
        ss << structName << "[" << std::to_string(index) << "]" << "." << fieldName;
        return glGetUniformLocation(programId, ss.str().c_str());
    }

    GLint GLShader::getUniformStructLocation(const char *structName, const char *fieldName) {
        std::stringstream ss;
        ss << structName << "." << fieldName;
        return glGetUniformLocation(programId, ss.str().c_str());
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
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniformMatrix4fv(location, 1, GL_FALSE, uniform.toFloatPtr());
    }

    void GLShader::setUniformArrayElement(const uint32_t &index, Mat4fUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        glUniformMatrix4fv(getUniformArrayElementLocation(uniform.name, index), 1, GL_FALSE, uniform.toFloatPtr());
    }

    void GLShader::setUniformArrayElement(const uint32_t &index, IntUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        glUniform1i(getUniformArrayElementLocation(uniform.name, index), uniform.value);
    }

    void GLShader::setUniformArrayElement(const uint32_t &index, FloatUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        glUniform1f(getUniformArrayElementLocation(uniform.name, index), uniform.value);
    }

    void GLShader::setUniformArrayStructField(const uint32_t &index, const char* structName, FloatUniform &structField) {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform1f(getUniformArrayStructLocation(structName, structField.name, index), structField.value);
    }

    void GLShader::setUniformArrayStructField(const uint32_t &index, const char *structName, Vec3fUniform &structField) {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform3fv(getUniformArrayStructLocation(structName, structField.name, index), 1, structField.toFloatPtr());
    }

    void GLShader::setUniformArrayStructField(const uint32_t &index, const char *structName, Vec4fUniform &structField) {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform4fv(getUniformArrayStructLocation(structName, structField.name, index), 1, structField.toFloatPtr());
    }

    void GLShader::setUniformStructField(const char *structName, FloatUniform &structField) {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform1f(getUniformStructLocation(structName, structField.name), structField.value);
    }

    void GLShader::setUniformStructField(const char *structName, Vec3fUniform &structField) {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform3fv(getUniformStructLocation(structName, structField.name), 1, structField.toFloatPtr());
    }

    void GLShader::setUniformStructField(const char *structName, Vec4fUniform &structField) {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform4fv(getUniformStructLocation(structName, structField.name), 1, structField.toFloatPtr());
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
        delete vUniformBlockFormat;
        delete fUniformBlockFormat;
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

    void GLShader::findUniformBlocks(const GLenum &shaderType, UniformBlockFormat* uniformBlockFormat, Ref<UniformBuffer>& uniformBuffer) {
        auto shader = _typeSources[shaderType];
        auto shaderTypeStr = toStringShaderType(shaderType);
        ENGINE_INFO("{0} shader is trying to find uniform blocks...", shaderTypeStr);
        auto shaderTokens = split(shader, "\r\n; ");

        auto uniformStructs = std::unordered_map<std::string, UniformStructAttribute>();
        // find all structs and uniform blocks!
        for (auto i = 0; i < shaderTokens.size(); i++) {
            if (shaderTokens[i] == "struct") {
                auto uniformStructName = shaderTokens[i + 1];
                auto uniformStruct = UniformStructAttribute { uniformStructName };
                ENGINE_INFO("Found struct {0} !", uniformStructName);
                // find all attrs in struct!
                auto uniformStructAttrs = std::vector<UniformAttribute>();
                auto j = i + 2;
                while (shaderTokens[j + 1] != "}") {
                    auto attrElementToken = shaderTokens[j + 1];
                    auto attrElementCount = toElementCount(attrElementToken);
                    if (attrElementCount == NONE) {
                        //todo we don't handle inner structs for now!
                        continue;
                    }

                    auto attrName = shaderTokens[j + 2];
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

                    ENGINE_INFO("Adding uniform attribute {0} {1} to struct {2}", attrElementToken, attrName, uniformStructName);
                    uniformStructAttrs.emplace_back(attr);

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
                uniformBlockFormat->setName(uniformBlockName);

                auto j = i + 2; // start of block {
                while (shaderTokens[j + 1] != "}") {
                    auto attrElementToken = shaderTokens[j + 1];
                    auto attrElementCount = toElementCount(attrElementToken);
                    //check if element is struct
                    if (attrElementCount == NONE) {
                        // check if struct exists in cache
                        if (uniformStructs.find(attrElementToken) != uniformStructs.end()) {
                            auto uniformStruct = uniformStructs[attrElementToken];
                            uniformBlockFormat->add(uniformStruct.uniformAttributes);
                        }
                        continue;
                    }

                    auto attrName = shaderTokens[j + 2];
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

                    uniformBlockFormat->add(attr);

                    j += 2;
                }
            }
            // if we found function main(), then we can stop searching, as all data should be declared before main() function!
            if (shaderTokens[i] == "void" && shaderTokens[i + 1] == "main()") {
                ENGINE_INFO("Found void main() function. Ending searching of uniform blocks!");
                break;
            }
        }

        if (uniformBlockFormat->isEmpty()) {
            ENGINE_WARN("{0} shader {1} doesn't has uniform blocks!", shaderTypeStr, props.name);
            state = NO_UNIFORM_BLOCKS;
            return;
        }

        uniformBuffer = createRef<GLUniformBuffer>();
        bindUniformBlock(uniformBlockFormat);
        uniformBuffer->prepare(uniformBlockFormat);

        ENGINE_INFO("{0} shader has found uniform blocks!", shaderTypeStr);
    }
}