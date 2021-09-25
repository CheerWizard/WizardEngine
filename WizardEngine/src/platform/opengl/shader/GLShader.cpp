//
// Created by mecha on 05.09.2021.
//

#include "GLShader.h"

#include <glm/gtc/type_ptr.hpp>

namespace engine {

    void GLShader::onCreate() {
        _typeSources[GL_VERTEX_SHADER] = readShader(props.vertexPath);
        _typeSources[GL_FRAGMENT_SHADER] = readShader(props.fragmentPath);
        compile();
    }

    void GLShader::start() {
        glUseProgram(programId);
    }

    void GLShader::stop() {
        glUseProgram(0);
    }

    void GLShader::bindAttributes() {
        for (Attribute &attribute : vertexFormat->getAttributes()) {
            auto attributeLocation = glGetAttribLocation(programId, attribute.name);
            glBindAttribLocation(programId, attributeLocation, attribute.name);
            attribute.location = attributeLocation;
        }
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
        glUniform2f(location, value.x, value.y);
    }

    void GLShader::setUniform(Vec3fUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        auto value = uniform.value;
        glUniform3f(location, value.x, value.y, value.z);
    }

    void GLShader::setUniform(Vec4fUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        auto value = uniform.value;
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void GLShader::setUniform(Mat2fUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(uniform.value));
    }

    void GLShader::setUniform(Mat3fUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(uniform.value));
    }

    void GLShader::setUniform(Mat4fUniform &uniform) {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniform.value));
    }

    void GLShader::compile() {
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
                ENGINE_ASSERT(false, shaderStringType + " shader compilation failure!")

                return;
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
            ENGINE_ASSERT(false, "Shader link failure!")

            delete this;
            return;
        }

        ENGINE_INFO("Shader program id={0} has been fully compiled!", programId);
    }

    std::string GLShader::toStringShaderType(GLenum type) {
        switch (type) {
            case GL_VERTEX_SHADER:
                return "Vertex";

                case GL_FRAGMENT_SHADER:
                    return "Fragment";

                    default: ENGINE_ASSERT(false, "Cannot convert shader type to string!")
        }
    }

    void GLShader::onDestroy() {
        detachShaders();
        deleteShaders();
        glDeleteProgram(programId);
        _shaderIds.clear();
        _typeSources.clear();
        delete vertexFormat;
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
}