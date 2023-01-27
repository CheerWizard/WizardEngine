//
// Created by mecha on 01.09.2022.
//

#pragma once

#include <imgui.h>
#include <core/primitives.h>
#include <core/core.h>

namespace engine::visual {

    using namespace core;

    class ENGINE_API Console final {

    public:
        Console();
        ~Console();

        static Console& get() {
            static Console instance;
            return instance;
        }

        void clearLog();
        void clearHistory();
        void addLog(const char* fmt, ...) IM_FMTARGS(2);
        void draw(float&& width, float&& height);
        void execCommand(const char* command_line);

    private:
        int textEditCallback(ImGuiInputTextCallbackData* data);
        static int textEditCallbackStub(ImGuiInputTextCallbackData* data);
        // Portable helpers
        static int Stricmp(const char* s1, const char* s2);
        static int Strnicmp(const char* s1, const char* s2, int n);
        static char* Strdup(const char* s);
        static void Strtrim(char* s);

        char InputBuf[256];
        ImVector<char*> Items;
        ImVector<const char*> Commands;
        ImVector<char*> History;
        int HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
        ImGuiTextFilter Filter;
        bool AutoScroll;
        bool ScrollToBottom;
        float width = 1024;
        float height = 768;
    };

}