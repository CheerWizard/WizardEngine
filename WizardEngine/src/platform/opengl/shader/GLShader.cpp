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

    void GLShader::prepare() {
        for (Attribute &attribute : vertex->getAttributes()) {
            auto attributeLocation = glGetAttribLocation(programId, attribute.name);
            glBindAttribLocation(programId, attributeLocation, attribute.name);
            attribute.location = attributeLocation;
        }
    }

    void GLShader::setUniform(const char* name, const float &value) {
        auto location = glGetUniformLocation(programId, name);
        glUniform1f(location, value);
    }

    void GLShader::setUniform(const char* name, const int &value) {
        auto location = glGetUniformLocation(programId, name);
        glUniform1i(location, value);
    }

    void GLShader::setUniform(const char* name, const double &value) {
        auto location = glGetUniformLocation(programId, name);
        glUniform1d(location, value);
    }

    void GLShader::setUniform(const char* name, const glm::fvec2 &value) {
        auto location = glGetUniformLocation(programId, name);
        glUniform2f(location, value.x, value.y);
    }

    void GLShader::setUniform(const char* name, const glm::fvec3 &value) {
        auto location = glGetUniformLocation(programId, name);
        glUniform3f(location, value.x, value.y, value.z);
    }

    void GLShader::setUniform(const char* name, const glm::fvec4 &value) {
        auto location = glGetUniformLocation(programId, name);
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void GLShader::setUniform(const char* name, const glm::fmat2 &value) {
        auto location = glGetUniformLocation(programId, name);
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void GLShader::setUniform(const char* name, const glm::fmat3 &value) {
        auto location = glGetUniformLocation(programId, name);
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void GLShader::setUniform(const Mat4fUniform &mat4Uniform) {
        auto location = glGetUniformLocation(programId, mat4Uniform.name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4Uniform.value));
    }

    void GLShader::compile() {
        programId = glCreateProgram();
        _shaderIds = std::vector<GLenum>(_typeSources.size());

        for (auto& typeSource : _typeSources) {
            GLenum type = typeSource.first;
            const std::string& source = typeSource.second;

            GLuint shader = glCreateShader(type);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

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

        detachShaders();
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
        glDeleteProgram(programId);
        deleteShaders();
        _shaderIds.clear();
        _typeSources.clear();
        delete vertex;
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