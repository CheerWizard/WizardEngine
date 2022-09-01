//
// Created by mecha on 05.09.2021.
//

#include <platform/graphics/Shader.h>
#include <io/Logger.h>
#include <glad/glad.h>
#include "sstream"

namespace engine::shader {

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

    std::string ShaderProgram::toStringShaderType(const u32& type) {
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

    GLint ShaderProgram::getUniformArrayElementLocation(const char* name, const uint32_t &index) const {
        std::stringstream ss;
        ss << name << "[" << std::to_string(index) << "]";
        return glGetUniformLocation(id, ss.str().c_str());
    }

    GLint ShaderProgram::getUniformArrayStructLocation(const char* structName, const char* fieldName, const uint32_t &index) const {
        std::stringstream ss;
        ss << structName << "[" << std::to_string(index) << "]" << "." << fieldName;
        return glGetUniformLocation(id, ss.str().c_str());
    }

    GLint ShaderProgram::getUniformStructLocation(const char *structName, const char *fieldName) const {
        std::stringstream ss;
        ss << structName << "." << fieldName;
        return glGetUniformLocation(id, ss.str().c_str());
    }

    void ShaderProgram::setUniform(FloatUniform &uniform) const {
        auto location = glGetUniformLocation(id, uniform.name);
        glUniform1f(location, uniform.value);
    }

    void ShaderProgram::setUniform(BoolUniform &uniform) const {
        auto location = glGetUniformLocation(id, uniform.name);
        glUniform1i(location, uniform.value); // bool is 32-bit as integer in GLSL compiler.
    }

    void ShaderProgram::setUniform(IntUniform &uniform) const {
        auto location = glGetUniformLocation(id, uniform.name);
        glUniform1i(location, uniform.value);
    }

    void ShaderProgram::setUniform(DoubleUniform &uniform) const {
        auto location = glGetUniformLocation(id, uniform.name);
        glUniform1d(location, uniform.value);
    }

    void ShaderProgram::setUniform(Vec2fUniform &uniform) const {
        auto location = glGetUniformLocation(id, uniform.name);
        glUniform2fv(location, 1, toFloatPtr(uniform));
    }

    void ShaderProgram::setUniform(Vec3fUniform &uniform) const {
        auto location = glGetUniformLocation(id, uniform.name);
        glUniform3fv(location, 1, toFloatPtr(uniform));
    }

    void ShaderProgram::setUniform(GLMVec3fUniform &uniform) const {
        auto location = glGetUniformLocation(id, uniform.name);
        glUniform3fv(location, 1, glm_toFloatPtr(uniform));
    }

    void ShaderProgram::setUniform(Vec4fUniform &uniform) const {
        auto location = glGetUniformLocation(id, uniform.name);
        glUniform4fv(location, 1, toFloatPtr(uniform));
    }

    void ShaderProgram::setUniform(Mat2fUniform &uniform) const {
        auto location = glGetUniformLocation(id, uniform.name);
        glUniformMatrix2fv(location, 1, GL_FALSE, toFloatPtr(uniform));
    }

    void ShaderProgram::setUniform(Mat3fUniform &uniform) const {
        auto location = glGetUniformLocation(id, uniform.name);
        glUniformMatrix3fv(location, 1, GL_FALSE, toFloatPtr(uniform));
    }

    void ShaderProgram::setUniform(Mat4fUniform &uniform) const {
        auto location = glGetUniformLocation(id, uniform.name);
        glUniformMatrix4fv(location, 1, GL_FALSE, toFloatPtr(uniform));
    }

    void ShaderProgram::setUniform(GLMMat4fUniform &uniform) const {
        auto location = glGetUniformLocation(id, uniform.name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm_toFloatPtr(uniform));
    }

    void ShaderProgram::setUniformArrayElement(const uint32_t &index, Mat4fUniform &uniform) const {
        glUniformMatrix4fv(getUniformArrayElementLocation(uniform.name, index), 1, GL_FALSE, toFloatPtr(uniform));
    }

    void ShaderProgram::setUniformArrayElement(const u32 &index, GLMMat4fUniform &uniform) const {
        glUniformMatrix4fv(getUniformArrayElementLocation(uniform.name, index), 1, GL_FALSE, glm_toFloatPtr(uniform));
    }

    void ShaderProgram::setUniformArrayElement(const uint32_t &index, IntUniform &uniform) const {
        glUniform1i(getUniformArrayElementLocation(uniform.name, index), uniform.value);
    }

    void ShaderProgram::setUniformArrayElement(const uint32_t &index, FloatUniform &uniform) const {
        glUniform1f(getUniformArrayElementLocation(uniform.name, index), uniform.value);
    }

    void ShaderProgram::setUniformArrayElement(const uint32_t &index, BoolUniform &uniform) const {
        glUniform1i(getUniformArrayElementLocation(uniform.name, index), uniform.value);
    }

    void ShaderProgram::setUniformArrayElement(const uint32_t &index, Vec4fUniform &uniform) const {
        glUniform4fv(getUniformArrayElementLocation(uniform.name, index), 1, toFloatPtr(uniform));
    }

    void ShaderProgram::setUniformStructField(const char *structName, BoolUniform &structField) const {
        glUniform1i(getUniformStructLocation(structName, structField.name), structField.value);
    }

    void ShaderProgram::setUniformStructField(const char *structName, FloatUniform &structField) const {
        glUniform1f(getUniformStructLocation(structName, structField.name), structField.value);
    }

    void ShaderProgram::setUniformStructField(const char *structName, DoubleUniform &structField) const {
        glUniform1d(getUniformStructLocation(structName, structField.name), structField.value);
    }

    void ShaderProgram::setUniformStructField(const char *structName, IntUniform &structField) const {
        glUniform1i(getUniformStructLocation(structName, structField.name), structField.value);
    }

    void ShaderProgram::setUniformStructField(const char *structName, Vec3fUniform &structField) const {
        glUniform3fv(getUniformStructLocation(structName, structField.name), 1, toFloatPtr(structField));
    }

    void ShaderProgram::setUniformStructField(const char *structName, Vec4fUniform &structField) const {
        glUniform4fv(getUniformStructLocation(structName, structField.name), 1, toFloatPtr(structField));
    }

    void ShaderProgram::setUniformArrayStructField(const uint32_t &index, const char *structName, BoolUniform &structField) const {
        glUniform1i(getUniformArrayStructLocation(structName, structField.name, index), structField.value);
    }

    void ShaderProgram::setUniformArrayStructField(const uint32_t &index, const char *structName, IntUniform &structField) const {
        glUniform1i(getUniformArrayStructLocation(structName, structField.name, index), structField.value);
    }

    void ShaderProgram::setUniformArrayStructField(const uint32_t &index, const char* structName, FloatUniform &structField) const {
        glUniform1f(getUniformArrayStructLocation(structName, structField.name, index), structField.value);
    }

    void ShaderProgram::setUniformArrayStructField(const uint32_t &index, const char* structName, DoubleUniform &structField) const {
        glUniform1d(getUniformArrayStructLocation(structName, structField.name, index), structField.value);
    }

    void ShaderProgram::setUniformArrayStructField(const uint32_t &index, const char *structName, Vec3fUniform &structField) const {
        glUniform3fv(getUniformArrayStructLocation(structName, structField.name, index), 1, toFloatPtr(structField));
    }

    void ShaderProgram::setUniformArrayStructField(const uint32_t &index, const char *structName, Vec4fUniform &structField) const {
        glUniform4fv(getUniformArrayStructLocation(structName, structField.name, index), 1, toFloatPtr(structField));
    }

}