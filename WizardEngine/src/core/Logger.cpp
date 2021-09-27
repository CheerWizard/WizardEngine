//
// Created by mecha on 29.08.2021.
//

#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace engine {

    Ref<spdlog::logger> Logger::_clientLogger;
    Ref<spdlog::logger> Logger::_engineLogger;

    void Logger::createEngineLogger(const std::string &name = "WizardEngine") {
        auto logSinks = createLogSinks(name);
        _engineLogger = createRef<spdlog::logger>(name, std::begin(logSinks), std::end(logSinks));
        spdlog::register_logger(_engineLogger);
        _engineLogger->set_level(spdlog::level::trace);
        _engineLogger->flush_on(spdlog::level::trace);
    }

    void Logger::createClientLogger(const std::string &name = "WizardClient") {
        auto logSinks = createLogSinks(name);
        _clientLogger = createRef<spdlog::logger>(name, std::begin(logSinks), std::end(logSinks));
        spdlog::register_logger(_clientLogger);
        _clientLogger->set_level(spdlog::level::trace);
        _clientLogger->flush_on(spdlog::level::trace);
    }

    void Logger::setPattern(const std::string& pattern) {
        spdlog::set_pattern(pattern);
    }

    std::vector<spdlog::sink_ptr> Logger::createLogSinks(const std::string &logFileName) {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(createRef<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(createRef<spdlog::sinks::basic_file_sink_mt>(logFileName + ".log", true));

        logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        return logSinks;
    }

}