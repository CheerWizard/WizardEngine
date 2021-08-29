//
// Created by mecha on 29.08.2021.
//

#include "Logger.h"

void engine::Logger::createEngineLogger(const std::string &name) {
    engineLogger = spdlog::stdout_color_mt(name);
}

void engine::Logger::createClientLogger(const std::string &name) {
    clientLogger = spdlog::stdout_color_mt(name);
}

void engine::Logger::setPattern(const std::string& pattern) {
    spdlog::set_pattern(pattern);
}