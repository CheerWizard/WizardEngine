//
// Created by mecha on 05.09.2021.
//

#include "GLShader.h"

#include <glm/gtc/type_ptr.hpp>

namespace engine {

    GLenum GLShader::toShaderType(const std::string &type) {
        if (type == "vertex") {
            return GL_VERTEX_SHADER;
        }

        if (type == "fragment") {
            return GL_FRAGMENT_SHADER;
        }

        ENGINE_ASSERT(false, "Unknown string shader type!")
        return 0;
    }

    std::string GLShader::toStringShaderType(const GLenum &type) {
        if (type == GL_VERTEX_SHADER) {
            return "vertex";
        }

        if (type == GL_FRAGMENT_SHADER) {
            return "fragment";
        }

        ENGINE_ASSERT(false, "Unknown shader type!")
        return "";
    }

    bool GLShader::isTypeValid(const std::string& type) {
        return type == "vertex" || type == "fragment";
    }

    void GLShader::onCreate() {
        auto sources = read();
        getTypeSources(sources);
        compile();
    }

    void GLShader::onCreate(const std::string &vertexSrc, const std::string &fragmentSrc) {
        _typeSources[GL_VERTEX_SHADER] = vertexSrc;
        _typeSources[GL_FRAGMENT_SHADER] = fragmentSrc;
        compile();
    }

    void GLShader::bind() {
        glUseProgram(programId);
    }

    void GLShader::unbind() {
        glUseProgram(0);
    }

    void GLShader::prepare() {
        for (Attribute* attribute : vertex->getAttributes()) {
            auto attributeLocation = glGetAttribLocation(programId, attribute->name);
            glBindAttribLocation(programId, attributeLocation, attribute->name);
            attribute->location = attributeLocation;
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

    void GLShader::setUniform(const char* name, const glm::fmat4 &value) {
        auto location = glGetUniformLocation(programId, name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void GLShader::getTypeSources(const std::string &sources) {
        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = sources.find(typeToken, 0);

        while (pos != std::string::npos) {
            size_t eol = sources.find_first_of("\r\n", pos);
            ENGINE_ASSERT(eol != std::string::npos, "Syntax error!")

            size_t begin = pos + typeTokenLength + 1;
            std::string type = sources.substr(begin, eol - begin);
            ENGINE_ASSERT(isTypeValid(type), "Invalid shader type specification!")

            size_t nextLinePos = sources.find_first_not_of("\r\n", eol);
            pos = sources.find(type, nextLinePos);
            auto shaderType = toShaderType(type);
            _typeSources[shaderType] = sources.substr(nextLinePos, pos - (nextLinePos == std::string::npos));
        }
    }

    void GLShader::compile() {
        auto _programId = glCreateProgram();
        std::vector<GLenum> shaderIds(_typeSources.size());

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

            glAttachShader(_programId, shader);
            shaderIds.push_back(shader);
        }

        glLinkProgram(_programId);
        GLint isLinked = 0;
        glGetProgramiv(_programId, GL_LINK_STATUS, (int*) &isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(_programId, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(_programId);

            for (auto shaderId : shaderIds) {
                glDeleteShader(shaderId);
            }

            ENGINE_ERR("{0}", infoLog.data());
            ENGINE_ASSERT(false, "Shader link failure!")
            return;
        }

        for (auto shaderId : shaderIds) {
            glDetachShader(_programId, shaderId);
        }

        programId = _programId;
    }

}