//
// Created by mecha on 29.08.2021.
//
#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <core/Memory.h>
#include <core/exception.h>

namespace engine::io {

    using namespace engine::core;

    typedef spdlog::logger Log;

    class ENGINE_API Logger final {

    public:
        static void createEngineLogger(const std::string& name);
        static void createEditorLogger(const std::string& name);
        static void createRuntimeLogger(const std::string& name);
        static void setPattern(const std::string& pattern);
        static void printException(const Ref<Log>& logger, const exception& exception);

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
        static void createLogger(Ref<Log> &logger, const std::string &name);

    public:
        static Ref<Log> _engineLogger;
        static Ref<Log> _editorLogger;
        static Ref<Log> _runtimeLogger;
    };

}

#ifdef DEBUG
    #define LOG_PATTERN(...)      ::engine::io::Logger::setPattern(__VA_ARGS__)

    // Engine log macros
    #define INIT_ENGINE_LOG(...)  ::engine::io::Logger::createEngineLogger(__VA_ARGS__)
    #define ENGINE_TRACE(...)     ::engine::io::Logger::getEngineLogger()->trace(__VA_ARGS__)
    #define ENGINE_INFO(...)      ::engine::io::Logger::getEngineLogger()->info(__VA_ARGS__)
    #define ENGINE_WARN(...)      ::engine::io::Logger::getEngineLogger()->warn(__VA_ARGS__)
    #define ENGINE_ERR(...)       ::engine::io::Logger::getEngineLogger()->error(__VA_ARGS__)
    #define ENGINE_CRIT(...)      ::engine::io::Logger::getEngineLogger()->critical(__VA_ARGS__)
    #define ENGINE_EXCEPT(ex)     ::engine::io::Logger::printException(::engine::io::Logger::getEngineLogger(), ex)
    #define ENGINE_THROW(ex)      ENGINE_EXCEPT(ex); throw (ex)

    // Runtime log macros
    #define INIT_RUNTIME_LOG(...)  ::engine::io::Logger::createRuntimeLogger(__VA_ARGS__)
    #define RUNTIME_TRACE(...)     ::engine::io::Logger::getRuntimeLogger()->trace(__VA_ARGS__)
    #define RUNTIME_INFO(...)      ::engine::io::Logger::getRuntimeLogger()->info(__VA_ARGS__)
    #define RUNTIME_WARN(...)      ::engine::io::Logger::getRuntimeLogger()->warn(__VA_ARGS__)
    #define RUNTIME_ERR(...)       ::engine::io::Logger::getRuntimeLogger()->error(__VA_ARGS__)
    #define RUNTIME_CRIT(...)      ::engine::io::Logger::getRuntimeLogger()->critical(__VA_ARGS__)
    #define RUNTIME_EXCEPT(ex)     ::engine::io::Logger::printException(::engine::io::Logger::getRuntimeLogger(), ex)
    #define RUNTIME_THROW(ex)      RUNTIME_EXCEPT(ex); throw (ex)

    // Editor log macros
    #define INIT_EDITOR_LOG(...)  ::engine::io::Logger::createEditorLogger(__VA_ARGS__)
    #define EDITOR_TRACE(...)     ::engine::io::Logger::getEditorLogger()->trace(__VA_ARGS__)
    #define EDITOR_INFO(...)      ::engine::io::Logger::getEditorLogger()->info(__VA_ARGS__)
    #define EDITOR_WARN(...)      ::engine::io::Logger::getEditorLogger()->warn(__VA_ARGS__)
    #define EDITOR_ERR(...)       ::engine::io::Logger::getEditorLogger()->error(__VA_ARGS__)
    #define EDITOR_CRIT(...)      ::engine::io::Logger::getEditorLogger()->critical(__VA_ARGS__)
    #define EDITOR_EXCEPT(ex)     ::engine::io::Logger::printException(::engine::io::Logger::getEditorLogger(), ex)
    #define EDITOR_THROW(ex)      EDITOR_EXCEPT(ex); throw (ex)

#else
    #define LOG_PATTERN(...)

    // Engine log macros
    #define INIT_ENGINE_LOG(...)
    #define ENGINE_TRACE(...)
    #define ENGINE_INFO(...)
    #define ENGINE_WARN(...)
    #define ENGINE_ERR(...)
    #define ENGINE_CRIT(...)
    #define ENGINE_EXCEPT(ex)
    #define ENGINE_THROW(ex) throw (ex)

    // Runtime log macros
    #define INIT_RUNTIME_LOG(...)
    #define RUNTIME_TRACE(...)
    #define RUNTIME_INFO(...)
    #define RUNTIME_WARN(...)
    #define RUNTIME_ERR(...)
    #define RUNTIME_CRIT(...)
    #define RUNTIME_EXCEPT(ex)
    #define RUNTIME_THROW(ex) throw (ex)

    // Editor log macros
    #define INIT_EDITOR_LOG(...)
    #define EDITOR_TRACE(...)
    #define EDITOR_INFO(...)
    #define EDITOR_WARN(...)
    #define EDITOR_ERR(...)
    #define EDITOR_CRIT(...)
    #define EDITOR_EXCEPT(ex)
    #define EDITOR_THROW(ex) throw (ex)
#endif