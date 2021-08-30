//
// Created by mecha on 29.08.2021.
//
#pragma once

#include "spdlog/spdlog.h"

#include "Core.h"
#include "Memory.h"

#include <string>

namespace engine {

    class Logger {

    public:
        static void createEngineLogger(const std::string& name);
        static void createClientLogger(const std::string& name);

    public:
        static void setPattern(const std::string& pattern);

    public:
        inline static Ref<spdlog::logger>& getEngineLogger() {
            return _engineLogger;
        }
        inline static Ref<spdlog::logger>& getClientLogger() {
            return _clientLogger;
        }

    private:
        static Ref<spdlog::logger> _engineLogger;
        static Ref<spdlog::logger> _clientLogger;

    };

}

#ifdef ENABLE_LOGS
    #define LOG_PATTERN(...)      ::engine::Logger::setPattern(__VA_ARGS__)

    // Engine log macros
    #define INIT_ENGINE_LOG(...)  ::engine::Logger::createEngineLogger(__VA_ARGS__)
    #define ENGINE_TRACE(...)     ::engine::Logger::getEngineLogger()->trace(__VA_ARSG__)
    #define ENGINE_INFO(...)      ::engine::Logger::getEngineLogger()->info(__VA_ARSG__)
    #define ENGINE_WARN(...)      ::engine::Logger::getEngineLogger()->warn(__VA_ARSG__)
    #define ENGINE_ERR(...)       ::engine::Logger::getEngineLogger()->error(__VA_ARSG__)
    #define ENGINE_CRIT(...)      ::engine::Logger::getEngineLogger()->critical(__VA_ARSG__)

    // Client log macros
    #define INIT_CLIENT_LOG(...)  ::engine::Logger::createClientLogger(__VA_ARGS__)
    #define CLIENT_TRACE(...)     ::engine::Logger::getClientLogger()->trace(__VA_ARSG__)
    #define CLIENT_INFO(...)      ::engine::Logger::getClientLogger()->info(__VA_ARSG__)
    #define CLIENT_WARN(...)      ::engine::Logger::getClientLogger()->warn(__VA_ARSG__)
    #define CLIENT_ERR(...)       ::engine::Logger::getClientLogger()->error(__VA_ARSG__)
    #define CLIENT_CRIT(...)      ::engine::Logger::getClientLogger()->critical(__VA_ARSG__)
#else
    #define LOG_PATTERN(...)

    // Engine log macros
    #define INIT_ENGINE_LOG(...)
    #define ENGINE_TRACE(...)
    #define ENGINE_INFO(...)
    #define ENGINE_WARN(...)
    #define ENGINE_ERR(...)
    #define ENGINE_CRIT(...)

    // Client log macros
    #define INIT_CLIENT_LOG(...)
    #define CLIENT_TRACE(...)
    #define CLIENT_INFO(...)
    #define CLIENT_WARN(...)
    #define CLIENT_ERR(...)
    #define CLIENT_CRIT(...)
#endif