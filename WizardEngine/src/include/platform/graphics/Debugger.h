#pragma once

#include <type_traits>
#include <core/primitives.h>

#define call(function, functionName, ...) engine::graphics::Debugger::callImpl(function, functionName, __VA_ARGS__)

namespace engine::graphics {

    using namespace core;

    enum class DebugSource {
        ANY, API, WINDOW, SHADER, THIRD_PARTY, APP, OTHER
    };

    enum class DebugType {
        ANY, ERROR, DEPRECATED,
        UNDEFINED, PORTABILITY,
        PERFORMANCE, MARKER,
        PUSH_GROUP, POP_GROUP,
        OTHER
    };

    enum class DebugSeverity {
        ANY, LOW, MEDIUM, HIGH, NOTIFICATION
    };

    struct DebugMessage {
        DebugSource source;
        DebugType type;
        u32 id = 0;
        DebugSeverity severity;
        int length = -1;
        const char* msg;

        DebugMessage(const DebugSource& source, const DebugType& type, const DebugSeverity& severity, const char* msg)
        : source(source), type(type), severity(severity), msg(msg) {}

        DebugMessage(const DebugSource& source, const DebugType& type,
                     u32 id, const DebugSeverity& severity,
                     size_t length, const char* msg
        ) : source(source), type(type), id(id), severity(severity), length(length), msg(msg) {}
    };

    class Debugger final {

    public:
        static void init();
        static bool checkError(const char* functionName);
        static void enableDebugOutput();
        static void disableDebugOutput();
        static void setFilter(const DebugSource& source, const DebugType& type, const DebugSeverity& severity);
        static void insert(const DebugMessage& debugMessage);

        template<typename Function, typename... Params>
        static auto callImpl(
                Function function,
                const char* functionName,
                Params... params
        )->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))> {
            auto result = function(std::forward<Params>(params)...);
#ifdef DEBUG
            checkError(functionName);
#endif
            return result;
        }

        template<typename Function, typename... Params>
        static auto callImpl(
                Function function,
                const char* functionName,
                Params... params
        )->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool> {
            function(std::forward<Params>(params)...);
            return checkError(functionName);
        }

    private:
        static bool available;
    };

}