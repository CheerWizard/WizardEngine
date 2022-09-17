//
// Created by mecha on 16.04.2022.
//

#pragma once

#include <core/String.h>

namespace engine::core {

    enum class exception_priority : u16 {
        TRACE = 0, INFO = 1, WARN = 2, ERR = 3, FATAL = 4
    };

    struct ENGINE_API exception {
        exception_priority priority = exception_priority::ERR;
        const char* message;
        u16 errorCode = 0;

        explicit exception(const char* message) : message(message) {}
        explicit exception(const char* message, u16 errorCode) : message(message), errorCode(errorCode) {}
        explicit exception(const std::string& message, u16 errorCode) : message(message.c_str()), errorCode(errorCode) {}
        explicit exception(const std::string& message) : message(message.c_str()) {}
        exception(exception_priority priority, const char* message) : priority(priority), message(message) {}
        exception(exception_priority priority, const std::string& message) : priority(priority), message(message.c_str()) {}
        exception(exception_priority priority, const char* message, u16 errorCode) : priority(priority), message(message), errorCode(errorCode) {}
        exception(exception_priority priority, const std::string& message, u16 errorCode) : priority(priority), message(message.c_str()), errorCode(errorCode) {}
        exception(const exception& exception) = default;
    };

    // helps declare new exception type
    #define decl_exception(name) \
    struct ENGINE_API name : engine::core::exception {    \
    explicit name(const char* message) : engine::core::exception(message) {} \
    explicit name(const std::string& message) : engine::core::exception(message) {} \
    name(engine::core::exception_priority priority, const char* message) : engine::core::exception(priority, message) {} \
    name(engine::core::exception_priority priority, const std::string& message) : engine::core::exception(priority, message) {} \
    explicit name(const char* message, u16 errorCode) : engine::core::exception(message, errorCode) {} \
    explicit name(const std::string& message, u16 errorCode) : engine::core::exception(message, errorCode) {} \
    name(engine::core::exception_priority priority, const char* message, u16 errorCode) : engine::core::exception(priority, message, errorCode) {} \
    name(engine::core::exception_priority priority, const std::string& message, u16 errorCode) : engine::core::exception(priority, message, errorCode) {} \
    };

    decl_exception(file_not_found)
}