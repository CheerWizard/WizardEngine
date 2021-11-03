//
// Created by mecha on 09.10.2021.
//

#include "FileEditor.h"

#include <imgui.h>

namespace engine {

    void FileEditor::open(const std::string &filePath) {
        setPath(filePath);
        _source = File::read(filePath);
        ENGINE_INFO("Opened file - path : {0}, source : {1}", _path, _source);
    }

    void FileEditor::onUpdate(Time dt) {
        if (!props.isVisible) return;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0 , 0});

        if (!ImGui::Begin(props.title.c_str(), &props.isVisible)) {
            end();
            return;
        }

        if (ImGui::BeginMenuBar()) {

            if (ImGui::BeginMenu("File")) {

                if (ImGui::MenuItem("Save")) {
                    save();
                }

                if (ImGui::MenuItem("Exit")) {
                    props.isVisible = false;
                    ImGui::EndMenu();
                    ImGui::EndMenuBar();
                    ImGui::End();
                    return;
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImVec2 textFieldSize = ImGui::GetContentRegionAvail();
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

        textFieldFont->FontSize = 24.0f;
        ImGui::PushFont(textFieldFont);

        char buffer[1000];
        auto bufferSize = sizeof(buffer);
        memset(buffer, 0, bufferSize);
        std::strncpy(buffer, _source.c_str(), bufferSize);

        if (ImGui::InputTextMultiline("", buffer, bufferSize, textFieldSize, flags)) {
            _source = std::string(buffer);
        }

        ImGui::PopFont();

        end();
    }

    void FileEditor::save() {
        File::write(_path, _source);
    }

    void FileEditor::destroy() {
        textFieldFont = nullptr;
    }

    void FileEditor::end() {
        ImGui::End();
        ImGui::PopStyleVar();
    }

}