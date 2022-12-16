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

    private:
        Log() = default;
        ~Log();

    public:
        static Log& get() {
            static Log instance;
            return instance;
        }

        void clear();
        void draw();
        void add(LogLevel logLevel, const std::string& fmt, ...);

    public:
        bool autoScroll = false;
        bool enabled = false;

    private:
        ImGuiTextBuffer buffer;
        ImGuiTextFilter filter;
        ImVector<int> lineOffsets;
        core::vector<LogLevel> levels;
        int maxLines = 100;
        std::string maxLinesStr = "100";
    };

}