//
// Created by mecha on 29.08.2021.
//

#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace engine {

    Ref<Log> Logger::_runtimeLogger = nullptr;
    Ref<Log> Logger::_engineLogger = nullptr;
    Ref<Log> Logger::_editorLogger = nullptr;

    void Logger::createEngineLogger(const std::string &name) {
        if (_engineLogger != nullptr) {
            ENGINE_WARN("Engine logger is already created!");
            return;
        }
        createLogger(_engineLogger, name);
    }

    void Logger::createRuntimeLogger(const std::string &name) {
        if (_runtimeLogger != nullptr) {
            ENGINE_WARN("Runtime logger is already created!");
            return;
        }
        createLogger(_runtimeLogger, name);
    }

    void Logger::createEditorLogger(const std::string &name) {
        if (_editorLogger != nullptr) {
            ENGINE_WARN("Editor logger is already created!");
            return;
        }
        createLogger(_editorLogger, name);
    }

    void Logger::setPattern(const std::string& pattern) {
        spdlog::set_pattern(pattern);
    }

    void Logger::createLogger(Ref<Log> &logger, const std::string &name) {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(createRef<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(createRef<spdlog::sinks::basic_file_sink_mt>(name + ".log", true));

        logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        logger = createRef<Log>(name, std::begin(logSinks), std::end(logSinks));

        spdlog::register_logger(logger);
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);
    }
}