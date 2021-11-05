//
// Created by mecha on 29.08.2021.
//
#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include "Memory.h"
#include "String.h"

//todo figure out logs optimization! debug: CPU intensity is 15-30%, release: CPU intensity is only 1%!
namespace engine {

    typedef spdlog::logger Log;

    class Logger {

    public:
        static void createEngineLogger(const std::string& name);
        static void createEditorLogger(const std::string& name);
        static void createRuntimeLogger(const std::string& name);

    public:
        static void setPattern(const std::string& pattern);

    public:
        static Ref<spdlog::logger>& getEngineLogger() {
            return _engineLogger;
        }

        static Ref<spdlog::logger>& getRuntimeLogger() {
            return _runtimeLogger;
        }

        static Ref<spdlog::logger>& getEditorLogger() {
            return _editorLogger;
        }

    private:
        // logger - should be NULL or undefined, as it will be created in scope of this function!
        static void createLogger(Ref<Log> &logger, const std::string &name);

    private:
        static Ref<Log> _engineLogger;
        static Ref<Log> _editorLogger;
        static Ref<Log> _runtimeLogger;

    };

}

#ifdef DEBUG
    #define LOG_PATTERN(...)      ::engine::Logger::setPattern(__VA_ARGS__)

    // Engine log macros
    #define INIT_ENGINE_LOG(...)  ::engine::Logger::createEngineLogger(__VA_ARGS__)
    #define ENGINE_TRACE(...)     ::engine::Logger::getEngineLogger()->trace(__VA_ARGS__)
    #define ENGINE_INFO(...)      ::engine::Logger::getEngineLogger()->info(__VA_ARGS__)
    #define ENGINE_WARN(...)      ::engine::Logger::getEngineLogger()->warn(__VA_ARGS__)
    #define ENGINE_ERR(...)       ::engine::Logger::getEngineLogger()->error(__VA_ARGS__)
    #define ENGINE_CRIT(...)      ::engine::Logger::getEngineLogger()->critical(__VA_ARGS__)

    // Runtime log macros
    #define INIT_RUNTIME_LOG(...)  ::engine::Logger::createRuntimeLogger(__VA_ARGS__)
    #define RUNTIME_TRACE(...)     ::engine::Logger::getRuntimeLogger()->trace(__VA_ARGS__)
    #define RUNTIME_INFO(...)      ::engine::Logger::getRuntimeLogger()->info(__VA_ARGS__)
    #define RUNTIME_WARN(...)      ::engine::Logger::getRuntimeLogger()->warn(__VA_ARGS__)
    #define RUNTIME_ERR(...)       ::engine::Logger::getRuntimeLogger()->error(__VA_ARGS__)
    #define RUNTIME_CRIT(...)      ::engine::Logger::getRuntimeLogger()->critical(__VA_ARGS__)

    // Editor log macros
    #define INIT_EDITOR_LOG(...)  ::engine::Logger::createEditorLogger(__VA_ARGS__)
    #define EDITOR_TRACE(...)     ::engine::Logger::getEditorLogger()->trace(__VA_ARGS__)
    #define EDITOR_INFO(...)      ::engine::Logger::getEditorLogger()->info(__VA_ARGS__)
    #define EDITOR_WARN(...)      ::engine::Logger::getEditorLogger()->warn(__VA_ARGS__)
    #define EDITOR_ERR(...)       ::engine::Logger::getEditorLogger()->error(__VA_ARGS__)
    #define EDITOR_CRIT(...)      ::engine::Logger::getEditorLogger()->critical(__VA_ARGS__)
#else
    #define LOG_PATTERN(...) __VA_ARGS__

    // Engine log macros
    #define INIT_ENGINE_LOG(...)    __VA_ARGS__
    #define ENGINE_TRACE(...)       __VA_ARGS__
    #define ENGINE_INFO(...)        __VA_ARGS__
    #define ENGINE_WARN(...)        __VA_ARGS__
    #define ENGINE_ERR(...)         __VA_ARGS__
    #define ENGINE_CRIT(...)        __VA_ARGS__

    // Runtime log macros
    #define INIT_RUNTIME_LOG(...)  __VA_ARGS__
    #define RUNTIME_TRACE(...)     __VA_ARGS__
    #define RUNTIME_INFO(...)      __VA_ARGS__
    #define RUNTIME_WARN(...)      __VA_ARGS__
    #define RUNTIME_ERR(...)       __VA_ARGS__
    #define RUNTIME_CRIT(...)      __VA_ARGS__

    // Editor log macros
    #define INIT_EDITOR_LOG(...)  __VA_ARGS__
    #define EDITOR_TRACE(...)     __VA_ARGS__
    #define EDITOR_INFO(...)      __VA_ARGS__
    #define EDITOR_WARN(...)      __VA_ARGS__
    #define EDITOR_ERR(...)       __VA_ARGS__
    #define EDITOR_CRIT(...)      __VA_ARGS__
#endif