//
// Created by mecha on 01.09.2022.
//

#pragma once

#include <imgui.h>
#include <core/primitives.h>
#include <core/core.h>

namespace engine::visual {

    using namespace core;

    struct ENGINE_API ConsoleProps {
        f32 width, height;
    };

    struct ENGINE_API ConsoleData {
        char                  InputBuf[256];
        ImVector<char*>       Items;
        ImVector<const char*> Commands;
        ImVector<char*>       History;
        int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
        ImGuiTextFilter       Filter;
        bool                  AutoScroll;
        bool                  ScrollToBottom;
        ConsoleProps props;

        ConsoleData() = default;

        void init();
        void release();
        // Portable helpers
        static int Stricmp(const char* s1, const char* s2);
        static int Strnicmp(const char* s1, const char* s2, int n);
        static char* Strdup(const char* s);
        static void Strtrim(char* s);

        void clearLog();
        void addLog(const char* fmt, ...) IM_FMTARGS(2);

        void draw(const char* title, const ConsoleProps& props);

        void execCommand(const char* command_line);

        static int textEditCallbackStub(ImGuiInputTextCallbackData* data);
        int textEditCallback(ImGuiInputTextCallbackData* data);
    };

    class ENGINE_API Console final {
    public:
        static void init();
        static void release();
        static void draw(const char* title, const ConsoleProps& props);
    private:
        static ConsoleData data;
    };

}