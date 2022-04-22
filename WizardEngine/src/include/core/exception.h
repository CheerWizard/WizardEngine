//
// Created by mecha on 16.04.2022.
//

#pragma once

#include <core/String.h>

namespace engine::core {

    enum class exception_priority : u8 {
        TRACE, INFO, WARN, ERROR, FATAL
    };

    struct exception {
        exception_priority priority = exception_priority::ERROR;
        const char* message;

        explicit exception(const char* message) : message(message) {}
        explicit exception(const std::string& message) : message(message.c_str()) {}
        exception(exception_priority priority, const char* message) : priority(priority), message(message) {}
        exception(exception_priority priority, const std::string& message) : priority(priority), message(message.c_str()) {}
        exception(const exception& exception) = default;
    };

    // helps declare new exception type
    #define decl_exception(name) \
    struct name : exception {    \
    explicit name(const char* message) : exception(message) {} \
    explicit name(const std::string& message) : exception(message) {} \
    name(exception_priority priority, const char* message) : exception(priority, message) {} \
    name(exception_priority priority, const std::string& message) : exception(priority, message) {} \
    };

    decl_exception(file_not_found)
}