//
// Created by mecha on 07.10.2022.
//

#pragma once

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <core/core.h>
#include <core/vector.h>
#include <imgui.h>

namespace engine::visual {

    enum LogLevel {
        L_TRACE, L_INFO, L_DEBUG, L_ERROR, L_WARN, L_CRITICAL
    };

    class ENGINE_API Log final {

    public:
        static void clear();
        static void draw();

        static void add(LogLevel logLevel, const std::string& fmt, ...);

    public:
        static bool                autoScroll;
        static bool                enabled;

    private:
        static ImGuiTextBuffer     _buf;
        static ImGuiTextFilter     filter;
        static ImVector<int>       lineOffsets;
        static core::vector<LogLevel> levels;
        static int maxLines;
        static std::string maxLinesStr;
    };

}