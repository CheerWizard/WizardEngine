//
// Created by mecha on 29.08.2021.
//

#include "spdlog/spdlog.h"

#include <memory>

namespace engine {

    class Logger {

    public:
        static void createEngineLogger(const std::string& name);
        static void createClientLogger(const std::string& name);

    public:
        static void setPattern(const std::string& pattern);

    public:
        inline static std::shared_ptr<spdlog::logger>& getEngineLogger() {
            return engineLogger;
        }
        inline static std::shared_ptr<spdlog::logger>& getClientLogger() {
            return clientLogger;
        }

    private:
        static std::shared_ptr<spdlog::logger>& engineLogger;
        static std::shared_ptr<spdlog::logger>& clientLogger;

    };

}

// Engine log macros
#define ENGINE_TRACE(...)     ::engine::Logger::getEngineLogger()->trace(__VA_ARSG__)
#define ENGINE_INFO(...)     ::engine::Logger::getEngineLogger()->info(__VA_ARSG__)
#define ENGINE_WARN(...)     ::engine::Logger::getEngineLogger()->warn(__VA_ARSG__)
#define ENGINE_ERR(...)     ::engine::Logger::getEngineLogger()->error(__VA_ARSG__)
#define ENGINE_CRIT(...)     ::engine::Logger::getEngineLogger()->critical(__VA_ARSG__)

// Client log macros
#define CLIENT_TRACE(...)     ::engine::Logger::getClientLogger()->trace(__VA_ARSG__)
#define CLIENT_INFO(...)     ::engine::Logger::getClientLogger()->info(__VA_ARSG__)
#define CLIENT_WARN(...)     ::engine::Logger::getClientLogger()->warn(__VA_ARSG__)
#define CLIENT_ERR(...)     ::engine::Logger::getClientLogger()->error(__VA_ARSG__)
#define CLIENT_CRIT(...)     ::engine::Logger::getClientLogger()->critical(__VA_ARSG__)