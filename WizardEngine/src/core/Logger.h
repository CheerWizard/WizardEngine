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

namespace engine {

    class Logger {

    public:
        static void createEngineLogger(const std::string& name);
        static void createClientLogger(const std::string& name);

    public:
        static void setPattern(const std::string& pattern);

    public:
        static Ref<spdlog::logger>& getEngineLogger() {
            return _engineLogger;
        }
        static Ref<spdlog::logger>& getClientLogger() {
            return _clientLogger;
        }

    private:
        static std::vector<spdlog::sink_ptr> createLogSinks(const std::string &logFileName);

    private:
        static Ref<spdlog::logger> _engineLogger;
        static Ref<spdlog::logger> _clientLogger;

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

    // Client log macros
    #define INIT_CLIENT_LOG(...)  ::engine::Logger::createClientLogger(__VA_ARGS__)
    #define CLIENT_TRACE(...)     ::engine::Logger::getClientLogger()->trace(__VA_ARGS__)
    #define CLIENT_INFO(...)      ::engine::Logger::getClientLogger()->info(__VA_ARGS__)
    #define CLIENT_WARN(...)      ::engine::Logger::getClientLogger()->warn(__VA_ARGS__)
    #define CLIENT_ERR(...)       ::engine::Logger::getClientLogger()->error(__VA_ARGS__)
    #define CLIENT_CRIT(...)      ::engine::Logger::getClientLogger()->critical(__VA_ARGS__)
#else
    #define LOG_PATTERN(...) __VA_ARGS__

    // Engine log macros
    #define INIT_ENGINE_LOG(...) __VA_ARGS__
    #define ENGINE_TRACE(...) __VA_ARGS__
    #define ENGINE_INFO(...) __VA_ARGS__
    #define ENGINE_WARN(...) __VA_ARGS__
    #define ENGINE_ERR(...) __VA_ARGS__
    #define ENGINE_CRIT(...) __VA_ARGS__

    // Client log macros
    #define INIT_CLIENT_LOG(...) __VA_ARGS__
    #define CLIENT_TRACE(...) __VA_ARGS__
    #define CLIENT_INFO(...) __VA_ARGS__
    #define CLIENT_WARN(...) __VA_ARGS__
    #define CLIENT_ERR(...) __VA_ARGS__
    #define CLIENT_CRIT(...) __VA_ARGS__
#endif