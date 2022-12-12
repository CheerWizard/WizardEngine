//
// Created by mecha on 01.09.2022.
//

#include <visual/Console.h>
#include <visual/FontAwesome4.h>
#include <imgui_stdlib.h>
#include <core/String.h>

namespace engine::visual {

    Console::Console() {
        clearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;
        Commands.push_back("/help");
        Commands.push_back("/history");
        Commands.push_back("/clear");
        Commands.push_back("/classify");
        AutoScroll = true;
        ScrollToBottom = false;
    }

    Console::~Console() {
        clearLog();
        for (int i = 0; i < History.Size; i++)
            free(History[i]);
    }

    int Console::Stricmp(const char* s1, const char* s2) {
        int d;
        while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
            s1++; s2++;
        }
        return d;
    }

    int Console::Strnicmp(const char* s1, const char* s2, int n) {
        int d = 0;
        while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
            s1++; s2++; n--;
        }
        return d;
    }

    char* Console::Strdup(const char* s) {
        IM_ASSERT(s);
        size_t len = strlen(s) + 1;
        void* buf = malloc(len);
        IM_ASSERT(buf);
        return (char*) memcpy(buf, (const void*)s, len);
    }

    void Console::Strtrim(char* s) {
        char* str_end = s + strlen(s);
        while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0;
    }

    void Console::clearLog() {
        for (int i = 0; i < Items.Size; i++)
            free(Items[i]);
        Items.clear();
    }

    void Console::addLog(const char* fmt, ...) {
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf)-1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
    }

    void Console::draw(float&& in_width, float&& in_height) {
        this->width = in_width;
        this->height = in_height;

        ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_FirstUseEver);
        static bool open = true;
        if (!ImGui::Begin(ICON_FA_TERMINAL "Console", &open)) {
            ImGui::End();
            return;
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Close Console"))
                open = false;
            ImGui::EndPopup();
        }

        ImGui::TextWrapped("Enter '/help' for help.");

        if (ImGui::SmallButton("Clear")) {
            clearLog();
        }
        ImGui::SameLine();

        bool copy_to_clipboard = ImGui::SmallButton("Copy");

        ImGui::Separator();

        // Options menu
        if (ImGui::BeginPopup("Options")) {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Options, Filter
        if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
        ImGui::SameLine();

        Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        ImGui::Separator();

        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::Selectable("Clear")) clearLog();
            ImGui::EndPopup();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
        if (copy_to_clipboard)
            ImGui::LogToClipboard();
        for (int i = 0; i < Items.Size; i++) {
            const char* item = Items[i];
            if (!Filter.PassFilter(item)) continue;

            // Normally you would store more information in your item than just a string.
            // (e.g. make Items[] an array of structure, store color/type etc.)
            ImVec4 color;

            bool has_color = false;
            if (strstr(item, "[error]")) {
                color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                has_color = true;
            } else if (strncmp(item, "# ", 2) == 0) {
                color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
                has_color = true;
            }

            if (has_color) ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextUnformatted(item);
            if (has_color) ImGui::PopStyleColor();
        }
        if (copy_to_clipboard) ImGui::LogFinish();

        if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
            ImGui::SetScrollHereY(1.0f);
        }
        ScrollToBottom = false;

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue |
                ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;

        ImGui::PushItemWidth(-1);
        if (ImGui::InputText("##label", InputBuf, IM_ARRAYSIZE(InputBuf),
                             input_text_flags, &textEditCallbackStub, (void*)this)) {
            char* s = InputBuf;
            Strtrim(s);
            if (s[0]) execCommand(s);
            strcpy(s, "");
            reclaim_focus = true;
        }
        ImGui::PopItemWidth();

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus) ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ImGui::End();
    }

    void Console::execCommand(const char* command_line) {
        addLog("# %s\n", command_line);
        // Insert into history. First find match and delete it so it can be pushed to the back.
        // This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.Size - 1; i >= 0; i--) {
            if (Stricmp(History[i], command_line) == 0) {
                free(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        }
        History.push_back(Strdup(command_line));
        // Process command
        SWITCH (command_line) {
            CASE("/clear"):
                clearLog();
                break;
            CASE("/help"):
                addLog("Commands:");
                for (int i = 0; i < Commands.Size; i++)
                    addLog("- %s", Commands[i]);
                break;
            CASE("/history"):
                int first = History.Size - 10;
                for (int i = first > 0 ? first : 0; i < History.Size; i++)
                    addLog("%3d: %s\n", i, History[i]);
                break;
        }
        // On command input, we scroll to bottom even if AutoScroll==false
        ScrollToBottom = true;
    }

    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
    int Console::textEditCallbackStub(ImGuiInputTextCallbackData* data) {
        auto* consoleData = (Console*)data->UserData;
        return consoleData->textEditCallback(data);
    }

    int Console::textEditCallback(ImGuiInputTextCallbackData* data) {
        //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag) {

            case ImGuiInputTextFlags_CallbackCompletion: {
                // Example of TEXT COMPLETION
                // Locate beginning of current word
                const char* word_end = data->Buf + data->CursorPos;
                const char* word_start = word_end;
                while (word_start > data->Buf) {
                    const char c = word_start[-1];
                    if (c == ' ' || c == '\t' || c == ',' || c == ';')
                        break;
                    word_start--;
                }
                // Build a list of candidates
                ImVector<const char*> candidates;
                for (int i = 0; i < Commands.Size; i++) {
                    if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0) {
                        candidates.push_back(Commands[i]);
                    }
                }

                if (candidates.Size == 0) {
                    // No match
                    addLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
                } else if (candidates.Size == 1) {
                    // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0]);
                    data->InsertChars(data->CursorPos, " ");
                } else {
                    // Multiple matches. Complete as much as we can..
                    // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                    int match_len = (int)(word_end - word_start);
                    for (;;) {
                        int c = 0;
                        bool all_candidates_matches = true;

                        for (int i = 0; i < candidates.Size && all_candidates_matches; i++) {
                            if (i == 0) c = toupper(candidates[i][match_len]);
                            else if (c == 0 || c != toupper(candidates[i][match_len])) all_candidates_matches = false;
                        }

                        if (!all_candidates_matches) break;
                        match_len++;
                    }

                    if (match_len > 0) {
                        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                        data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                    }

                    // List matches
                    addLog("Possible matches:\n");
                    for (int i = 0; i < candidates.Size; i++)
                        addLog("- %s\n", candidates[i]);
                }
                break;
            }

            case ImGuiInputTextFlags_CallbackHistory: {
                // Example of HISTORY
                const int prev_history_pos = HistoryPos;
                if (data->EventKey == ImGuiKey_UpArrow) {
                    if (HistoryPos == -1) HistoryPos = History.Size - 1;
                    else if (HistoryPos > 0) HistoryPos--;
                } else if (data->EventKey == ImGuiKey_DownArrow) {
                    if (HistoryPos != -1) {
                        if (++HistoryPos >= History.Size) HistoryPos = -1;
                    }
                }
                // A better implementation would preserve the data on the current input line along with cursor position.
                if (prev_history_pos != HistoryPos) {
                    const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, history_str);
                }
            }
        }
        return 0;
    }
};