//
// Created by mecha on 05.09.2021.
//

#include <platform/graphics/Shader.h>
#include <core/Logger.h>
#include <glad/glad.h>
#include "sstream"

namespace engine::shader {

    GLint Shader::getUniformArrayElementLocation(const char* name, const uint32_t &index) const {
        std::stringstream ss;
        ss << name << "[" << std::to_string(index) << "]";
        return glGetUniformLocation(programId, ss.str().c_str());
    }

    GLint Shader::getUniformArrayStructLocation(const char* structName, const char* fieldName, const uint32_t &index) const {
        std::stringstream ss;
        ss << structName << "[" << std::to_string(index) << "]" << "." << fieldName;
        return glGetUniformLocation(programId, ss.str().c_str());
    }

    GLint Shader::getUniformStructLocation(const char *structName, const char *fieldName) const {
        std::stringstream ss;
        ss << structName << "." << fieldName;
        return glGetUniformLocation(programId, ss.str().c_str());
    }

    void Shader::setUniform(FloatUniform &uniform) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniform1f(location, uniform.value);
    }

    void Shader::setUniform(BoolUniform &uniform) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniform1i(location, uniform.value); // bool is 32-bit as integer in GLSL compiler.
    }

    void Shader::setUniform(IntUniform &uniform) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniform1i(location, uniform.value);
    }

    void Shader::setUniform(DoubleUniform &uniform) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniform1d(location, uniform.value);
    }

    void Shader::setUniform(Vec2fUniform &uniform) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniform2fv(location, 1, toFloatPtr(uniform));
    }

    void Shader::setUniform(Vec3fUniform &uniform) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniform3fv(location, 1, toFloatPtr(uniform));
    }

    void Shader::setUniform(Vec4fUniform &uniform) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniform4fv(location, 1, toFloatPtr(uniform));
    }

    void Shader::setUniform(Mat2fUniform &uniform) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniformMatrix2fv(location, 1, GL_FALSE, toFloatPtr(uniform));
    }

    void Shader::setUniform(Mat3fUniform &uniform) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniformMatrix3fv(location, 1, GL_FALSE, toFloatPtr(uniform));
    }

    void Shader::setUniform(Mat4fUniform &uniform) const {
        if (!uniform.isUpdated) return;
        auto location = glGetUniformLocation(programId, uniform.name);
        glUniformMatrix4fv(location, 1, GL_FALSE, toFloatPtr(uniform));
    }

    void Shader::setUniformArrayElement(const uint32_t &index, Mat4fUniform &uniform) const {
        if (!uniform.isUpdated) return;
        glUniformMatrix4fv(getUniformArrayElementLocation(uniform.name, index), 1, GL_FALSE, toFloatPtr(uniform));
    }

    void Shader::setUniformArrayElement(const uint32_t &index, IntUniform &uniform) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        glUniform1i(getUniformArrayElementLocation(uniform.name, index), uniform.value);
    }

    void Shader::setUniformArrayElement(const uint32_t &index, FloatUniform &uniform) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        glUniform1f(getUniformArrayElementLocation(uniform.name, index), uniform.value);
    }

    void Shader::setUniformArrayElement(const uint32_t &index, Vec4fUniform &uniform) const {
        if (!uniform.isUpdated) return;
        uniform.isUpdated = false;
        glUniform4fv(getUniformArrayElementLocation(uniform.name, index), 1, toFloatPtr(uniform));
    }

    void Shader::setUniformStructField(const char *structName, BoolUniform &structField) const {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform1i(getUniformStructLocation(structName, structField.name), structField.value);
    }

    void Shader::setUniformStructField(const char *structName, FloatUniform &structField) const {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform1f(getUniformStructLocation(structName, structField.name), structField.value);
    }

    void Shader::setUniformStructField(const char *structName, IntUniform &structField) const {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform1i(getUniformStructLocation(structName, structField.name), structField.value);
    }

    void Shader::setUniformStructField(const char *structName, Vec3fUniform &structField) const {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform3fv(getUniformStructLocation(structName, structField.name), 1, toFloatPtr(structField));
    }

    void Shader::setUniformStructField(const char *structName, Vec4fUniform &structField) const {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform4fv(getUniformStructLocation(structName, structField.name), 1, toFloatPtr(structField));
    }

    void Shader::setUniformArrayStructField(const uint32_t &index, const char *structName, BoolUniform &structField) const {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform1i(getUniformArrayStructLocation(structName, structField.name, index), structField.value);
    }

    void Shader::setUniformArrayStructField(const uint32_t &index, const char *structName, IntUniform &structField) const {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform1i(getUniformArrayStructLocation(structName, structField.name, index), structField.value);
    }

    void Shader::setUniformArrayStructField(const uint32_t &index, const char* structName, FloatUniform &structField) const {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform1f(getUniformArrayStructLocation(structName, structField.name, index), structField.value);
    }

    void Shader::setUniformArrayStructField(const uint32_t &index, const char *structName, Vec3fUniform &structField) const {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform3fv(getUniformArrayStructLocation(structName, structField.name, index), 1, toFloatPtr(structField));
    }

    void Shader::setUniformArrayStructField(const uint32_t &index, const char *structName, Vec4fUniform &structField) const {
        if (!structField.isUpdated) return;
        structField.isUpdated = false;
        glUniform4fv(getUniformArrayStructLocation(structName, structField.name, index), 1, toFloatPtr(structField));
    }

    void Shader::bindUbf(const char *blockName, const uint32_t &blockIndex) const {
        auto uniformBlockIndex = glGetUniformBlockIndex(id, blockName);
        glUniformBlockBinding(id, uniformBlockIndex, blockIndex);
    }

    void Shader::createVShader() {
        id = glCreateShader(GL_VERTEX_SHADER);
    }

    void Shader::createFShader() {
        id = glCreateShader(GL_FRAGMENT_SHADER);
    }

    void Shader::createGShader() {
        id = glCreateShader(GL_GEOMETRY_SHADER);
    }

    void Shader::attach() const {
        glAttachShader(programId, id);
    }

    void Shader::detach() const {
        glDetachShader(programId, id);
    }

    void Shader::destroy() const {
        glDeleteShader(id);
    }

    bool Shader::compile() const {
        const GLchar* glSrc = src.c_str();
        glShaderSource(id, 1, &glSrc, nullptr);
        glCompileShader(id);

        GLint isCompiled = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(id);
            ENGINE_ERR("{0}", infoLog.data());
            return false;
        }

        return true;
    }

    void ShaderProgram::start() const {
        glUseProgram(id);
    }

    void ShaderProgram::stop() {
        glUseProgram(0);
    }

    std::string ShaderProgram::toStringShaderType(GLenum type) {
        switch (type) {
            case GL_VERTEX_SHADER:
                return "Vertex";
            case GL_FRAGMENT_SHADER:
                return "Fragment";
            default:
                ENGINE_ERR("Cannot convert shader type to string! programId : {0}", id);
                return "Undefined";
        }
    }

    uint32_t ShaderProgram::bindAttribute(const char* attrName) const {
        auto location = glGetAttribLocation(id, attrName);
        glBindAttribLocation(id, location, attrName);
        return location;
    }

    bool ShaderProgram::link() {
        glLinkProgram(id);
        GLint isLinked = 0;
        glGetProgramiv(id, GL_LINK_STATUS, (int*) &isLinked);

        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);

            ENGINE_ERR("{0}", infoLog.data());
            ENGINE_ERR("Shader link failure!");

            state = FAILED_LINKING;
            return false;
        }

        return true;
    }

    void ShaderProgram::create() {
        id = glCreateProgram();
    }

    void ShaderProgram::destroy() const {
        glDeleteProgram(id);
    }

}