//
// Created by mecha on 07.10.2022.
//

#include <visual/Log.h>
#include <visual/FontAwesome4.h>
#include <time/Time.h>
#include <visual/Widgets.h>

namespace engine::visual {

    Log::~Log() {
        clear();
    }

    void Log::clear() {
        buffer.clear();
        lineOffsets.clear();
        lineOffsets.push_back(0);
        levels.clear();
    }

    void Log::draw() {
        static bool open = true;
        if (!ImGui::Begin(ICON_FA_TERMINAL" Logs", &open)) {
            ImGui::End();
            return;
        }

        // Options menu
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
        ImGui::Checkbox("Auto-Scroll", &autoScroll);
        ImGui::SameLine();
        ImGui::Checkbox("Enabled", &enabled);
        ImGui::SameLine();
        Text::field("##logger-line-limit", "Limit", &maxLinesStr);
        ImGui::SameLine();
        ImGui::PopStyleVar();
        maxLines = maxLinesStr.empty() ? 0 : std::stoi(maxLinesStr);
        // Main window
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
        ImGui::SameLine();
        bool clearResult = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copyResult = ImGui::Button("Copy");
        ImGui::SameLine();
        filter.Draw(ICON_FA_FILTER" Filter", -100.0f);
        ImGui::PopStyleVar();

        ImGui::Separator();
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (clearResult) clear();
        if (copyResult)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = buffer.begin();
        const char* buf_end = buffer.end();
        if (filter.IsActive())
        {
            // In this example we don't use the clipper when Filter is enabled.
            // This is because we don't have a random access on the result on our filter.
            // A real application processing logs with ten of thousands of entries may want to store the result of
            // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
            for (int line_no = 0; line_no < lineOffsets.Size - 1; line_no++)
            {
                const char* line_start = buf + lineOffsets[line_no];
                const char* line_end = (line_no + 1 < lineOffsets.Size) ? (buf + lineOffsets[line_no + 1] - 1) : buf_end;
                if (filter.PassFilter(line_start, line_end)) {
                    // log level
                    switch (levels.at(line_no)) {
                        case L_TRACE:
                            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,255,255));
                            // local current date time
                            Text::label(time::currentLocalTime().c_str());
                            ImGui::SameLine();
                            Text::label("[TRACE]");
                            break;
                        case L_INFO:
                            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,255,0,255));
                            // local current date time
                            Text::label(time::currentLocalTime().c_str());
                            ImGui::SameLine();
                            Text::label("[INFO]");
                            break;
                        case L_DEBUG:
                            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,255,255));
                            // local current date time
                            Text::label(time::currentLocalTime().c_str());
                            ImGui::SameLine();
                            Text::label("[DEBUG]");
                            break;
                        case L_WARN:
                            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,0,255));
                            // local current date time
                            Text::label(time::currentLocalTime().c_str());
                            ImGui::SameLine();
                            Text::label("[WARN]");
                            break;
                        case L_ERROR:
                            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
                            // local current date time
                            Text::label(time::currentLocalTime().c_str());
                            ImGui::SameLine();
                            Text::label("[ERROR]");
                            break;
                        case L_CRITICAL:
                            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
                            // local current date time
                            Text::label(time::currentLocalTime().c_str());
                            ImGui::SameLine();
                            Text::label("[CRITICAL]");
                            break;
                    }
                    ImGui::SameLine();
                    // log message
                    ImGui::TextUnformatted(line_start, line_end);
                    ImGui::PopStyleColor();
                }
            }
        }
        else
        {
            // The simplest and easy way to display the entire buffer:
            //   ImGui::TextUnformatted(buf_begin, buf_end);
            // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
            // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
            // within the visible area.
            // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
            // on your side is recommended. Using ImGuiListClipper requires
            // - A) random access into your data
            // - B) items all being the  same height,
            // both of which we can handle since we an array pointing to the beginning of each line of text.
            // When using the filter (in the block of code above) we don't have random access into the data to display
            // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
            // it possible (and would be recommended if you want to search through tens of thousands of entries).
//            ImGuiListClipper clipper;
//            clipper.Begin(lineOffsets.Size);
            for (int line_no = 0; line_no < lineOffsets.Size - 1; line_no++)
            {
                // log level
                switch (levels.at(line_no)) {
                    case L_TRACE:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,255,255));
                        // local current date time
                        Text::label(time::currentLocalTime().c_str());
                        ImGui::SameLine();
                        Text::label("[TRACE]");
                        break;
                    case L_INFO:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,255,0,255));
                        // local current date time
                        Text::label(time::currentLocalTime().c_str());
                        ImGui::SameLine();
                        Text::label("[INFO]");
                        break;
                    case L_DEBUG:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,0,255,255));
                        // local current date time
                        Text::label(time::currentLocalTime().c_str());
                        ImGui::SameLine();
                        Text::label("[DEBUG]");
                        break;
                    case L_WARN:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,0,255));
                        // local current date time
                        Text::label(time::currentLocalTime().c_str());
                        ImGui::SameLine();
                        Text::label("[WARN]");
                        break;
                    case L_ERROR:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
                        // local current date time
                        Text::label(time::currentLocalTime().c_str());
                        ImGui::SameLine();
                        Text::label("[ERROR]");
                        break;
                    case L_CRITICAL:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,0,0,255));
                        // local current date time
                        Text::label(time::currentLocalTime().c_str());
                        ImGui::SameLine();
                        Text::label("[CRITICAL]");
                        break;
                }
                ImGui::SameLine();
                // log message
                const char* line_start = buf + lineOffsets[line_no];
                const char* line_end = (line_no + 1 < lineOffsets.Size) ? (buf + lineOffsets[line_no + 1] - 1) : buf_end;
                ImGui::TextUnformatted(line_start, line_end);
                ImGui::PopStyleColor();
            }
//            clipper.End();
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();

        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::End();
    }

    void Log::add(LogLevel level, const std::string& fmt, ...) {
        if (!enabled) return;

        if (lineOffsets.size() - 1 > maxLines) {
            clear();
        }

        levels.emplace_back(level);
        int old_size = buffer.size();

        std::string fmt_str = fmt + "\n";
        const char* fmt_c = fmt_str.c_str();
        va_list args;
        va_start(args, fmt_c);
        buffer.appendfv(fmt_c, args);
        va_end(args);

        for (int new_size = buffer.size(); old_size < new_size; old_size++)
            if (buffer[old_size] == '\n')
                lineOffsets.push_back(old_size + 1);
    }

}