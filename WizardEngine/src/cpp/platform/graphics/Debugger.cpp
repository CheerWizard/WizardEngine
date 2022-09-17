//
// Created by mecha on 03.09.2022.
//

#include <platform/graphics/Debugger.h>
#include <core/Assert.h>
#include <io/Logger.h>

#include <glad/glad.h>
#include <sstream>

namespace engine::graphics {

    bool Debugger::available;

    void Debugger::init() {
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        available = flags & GL_CONTEXT_FLAG_DEBUG_BIT;
    }

    bool Debugger::checkError(const char* functionName) {
        GLenum error = glGetError();

        if (error != GL_NO_ERROR) {
            std::stringstream msg;
            switch (error) {
                case GL_INVALID_VALUE:
                    msg << functionName << ": " << "GL_INVALID_VALUE";
                    break;
                case GL_INVALID_ENUM:
                    msg << functionName << ": " << "GL_INVALID_ENUM";
                    break;
                case GL_INVALID_INDEX:
                    msg << functionName << ": " << "GL_INVALID_INDEX";
                    break;
                case GL_INVALID_OPERATION:
                    msg << functionName << ": " << "GL_INVALID_OPERATION";
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    msg << functionName << ": " << "GL_INVALID_FRAMEBUFFER_OPERATION";
                    break;
                case GL_OUT_OF_MEMORY:
                    msg << functionName << ": " << "GL_OUT_OF_MEMORY";
                    break;
                case GL_STACK_OVERFLOW:
                    msg << functionName << ": " << "GL_STACK_OVERFLOW";
                    break;
                case GL_STACK_UNDERFLOW:
                    msg << functionName << ": " << "GL_STACK_UNDERFLOW";
                    break;
                default:
                    msg << functionName << ": " << "Unknown GL error code=" << error;
                    break;
            }
            ENGINE_ERR(msg.str());
            DEBUGBREAK();

            return false;
        }

        return true;
    }

    const char* GLSourceString(GLenum source) {
        switch (source) {
            case GL_DEBUG_SOURCE_API: return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD_PARTY";
            case GL_DEBUG_SOURCE_APPLICATION: return "APP";
            case GL_DEBUG_SOURCE_OTHER: return "OTHER";
            default: return "UNKNOWN";
        };
    }

    const char* GLTypeString(GLenum type) {
        switch (type) {
            case GL_DEBUG_TYPE_ERROR: return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED";
            case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER: return "MARKER";
            case GL_DEBUG_TYPE_PUSH_GROUP: return "PUSH_GROUP";
            case GL_DEBUG_TYPE_POP_GROUP: return "POP_GROUP";
            case GL_DEBUG_TYPE_OTHER: return "OTHER";
            default: return "UNKNOWN";
        }
    }

    const char* GLSeverityString(GLenum severity) {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
            case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
            case GL_DEBUG_SEVERITY_LOW: return "LOW";
            case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
            default: return "UNKNOWN";
        }
    }

    void APIENTRY glDebugOutput(
            GLenum source, GLenum type, unsigned int id, GLenum severity,
            GLsizei length, const char *message, const void *userParam) {

        ENGINE_INFO("----------- DEBUGGER ----------");
        ENGINE_INFO("[{0}][{1}][{2}] {3}: {4}",
                    GLSourceString(source),
                    GLTypeString(type),
                    GLSeverityString(severity),
                    id, message);
    }

    void Debugger::enableDebugOutput() {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
    }

    void Debugger::disableDebugOutput() {
        glDisable(GL_DEBUG_OUTPUT);
        glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(nullptr, nullptr);
    }

    GLenum toGLDebugSource(const DebugSource& source) {
        switch (source) {
            case DebugSource::ANY: return GL_DONT_CARE;
            case DebugSource::API: return GL_DEBUG_SOURCE_API;
            case DebugSource::WINDOW: return GL_DEBUG_SOURCE_WINDOW_SYSTEM;
            case DebugSource::SHADER: return GL_DEBUG_SOURCE_SHADER_COMPILER;
            case DebugSource::APP: return GL_DEBUG_SOURCE_APPLICATION;
            case DebugSource::THIRD_PARTY: return GL_DEBUG_SOURCE_THIRD_PARTY;
            case DebugSource::OTHER: return GL_DEBUG_SOURCE_OTHER;
        }
    }

    GLenum toGLDebugType(const DebugType& type) {
        switch (type) {
            case DebugType::ANY: return GL_DONT_CARE;
            case DebugType::ERROR: return GL_DEBUG_TYPE_ERROR;
            case DebugType::DEPRECATED: return GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR;
            case DebugType::MARKER: return GL_DEBUG_TYPE_MARKER;
            case DebugType::PERFORMANCE: return GL_DEBUG_TYPE_PERFORMANCE;
            case DebugType::UNDEFINED: return GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR;
            case DebugType::POP_GROUP: return GL_DEBUG_TYPE_POP_GROUP;
            case DebugType::PUSH_GROUP: return GL_DEBUG_TYPE_PUSH_GROUP;
            case DebugType::PORTABILITY: return GL_DEBUG_TYPE_PORTABILITY;
            case DebugType::OTHER: return GL_DEBUG_TYPE_OTHER;
        }
    }

    GLenum toGLDebugSeverity(const DebugSeverity& severity) {
        switch (severity) {
            case DebugSeverity::ANY: return GL_DONT_CARE;
            case DebugSeverity::LOW: return GL_DEBUG_SEVERITY_LOW;
            case DebugSeverity::MEDIUM: return GL_DEBUG_SEVERITY_MEDIUM;
            case DebugSeverity::HIGH: return GL_DEBUG_SEVERITY_HIGH;
            case DebugSeverity::NOTIFICATION: return GL_DEBUG_SEVERITY_NOTIFICATION;
        }
    }

    void Debugger::setFilter(const DebugSource &source, const DebugType &type, const DebugSeverity &severity) {
        glDebugMessageControl(
                toGLDebugSource(source),
                toGLDebugType(type),
                toGLDebugSeverity(severity),
                0, nullptr, GL_TRUE);
    }

    void Debugger::insert(const DebugMessage &debugMessage) {
        glDebugMessageInsert(toGLDebugSource(debugMessage.source),
                             toGLDebugType(debugMessage.type),
                             debugMessage.id,
                             toGLDebugSeverity(debugMessage.severity),
                             debugMessage.length,
                             debugMessage.msg);
    }

}