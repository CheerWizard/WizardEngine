//
// Created by mecha on 01.09.2022.
//

#include <visual/Widgets.h>
#include <visual/Theme.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace engine::visual {

    void Controller::draw(Vec2fUniform &uniform, float reset, float width) {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(uniform.name);

        ImGui::Columns(3);
        ImGui::SetColumnWidth(0, width);
        ImGui::Text("%s", uniform.name);
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, Theme::button);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Theme::buttonHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Theme::buttonPressed);
        ImGui::PushFont(boldFont);

        auto xClicked = ImGui::Button("X", buttonSize);
        if (xClicked) {
            uniform.value[0] = reset;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &uniform.value[0], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, Theme::button);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Theme::buttonHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Theme::buttonPressed);
        ImGui::PushFont(boldFont);

        auto yClicked = ImGui::Button("Y", buttonSize);
        if (yClicked) {
            uniform.value[1] = reset;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &uniform.value[1], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();
    }

    void Controller::draw(Vec3fUniform &uniform, float reset, float width) {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(uniform.name);

        ImGui::Columns(3);
        ImGui::SetColumnWidth(0, width);
        ImGui::Text("%s", uniform.name);
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, Theme::button);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Theme::buttonHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Theme::buttonPressed);
        ImGui::PushFont(boldFont);

        auto xClicked = ImGui::Button("X", buttonSize);
        if (xClicked) {
            uniform.value[0] = reset;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &uniform.value[0], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, Theme::button);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Theme::buttonHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Theme::buttonPressed);
        ImGui::PushFont(boldFont);

        auto yClicked = ImGui::Button("Y", buttonSize);
        if (yClicked) {
            uniform.value[1] = reset;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &uniform.value[1], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, Theme::button);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Theme::buttonHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Theme::buttonPressed);
        ImGui::PushFont(boldFont);

        auto zClicked = ImGui::Button("Z", buttonSize);
        if (zClicked) {
            uniform.value[2] = reset;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &uniform.value[2], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();
    }

    void Controller::draw(Vec4fUniform &uniform, float reset, float width) {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(uniform.name);

        ImGui::Columns(3);
        ImGui::SetColumnWidth(0, width);
        ImGui::Text("%s", uniform.name);
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, Theme::button);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Theme::buttonHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Theme::buttonPressed);
        ImGui::PushFont(boldFont);

        auto xClicked = ImGui::Button("X", buttonSize);
        if (xClicked) {
            uniform.value[0] = reset;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &uniform.value[0], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, Theme::button);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Theme::buttonHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Theme::buttonPressed);
        ImGui::PushFont(boldFont);

        auto yClicked = ImGui::Button("Y", buttonSize);
        if (yClicked) {
            uniform.value[1] = reset;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &uniform.value[1], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, Theme::button);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Theme::buttonHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Theme::buttonPressed);
        ImGui::PushFont(boldFont);

        auto zClicked = ImGui::Button("Z", buttonSize);
        if (zClicked) {
            uniform.value[2] = reset;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &uniform.value[2], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, Theme::button);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Theme::buttonHover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Theme::buttonPressed);
        ImGui::PushFont(boldFont);

        auto wClicked = ImGui::Button("W", buttonSize);
        if (wClicked) {
            uniform.value[3] = reset;
        }

        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##W", &uniform.value[3], 0.05f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();
    }

    void Checkbox::draw(BoolUniform &uniform) {
        ImGui::Checkbox(uniform.name, &uniform.value);
    }

    void Checkbox::draw(const char *name, bool &value) {
        ImGui::Checkbox(name, &value);
    }

    void Slider::draw(FloatUniform &uniform, const vec2f &range) {
        ImGui::SliderFloat(uniform.name, &uniform.value, range.x(), range.y());
    }

    void Slider::draw(IntUniform &uniform, const vec2i &range) {
        ImGui::SliderInt(uniform.name, &uniform.value, range.x(), range.y());
    }

    void Slider::draw(DoubleUniform &uniform, const vec2f &range) {
        ImGui::SliderFloat(uniform.name, (float*) &uniform.value, range.x(), range.y());
    }

    void Slider::draw(const char* name, float& value, const vec2f &range) {
        ImGui::SliderFloat(name, &value, range.x(), range.y());
    }

    void Slider::draw(const char* name, int& value, const vec2i &range) {
        ImGui::SliderInt(name, &value, range.x(), range.y());
    }

    void Slider::draw(const char* name, u32& value, const vec2i &range) {
        ImGui::SliderInt(name, (int*)&value, range.x(), range.y());
    }

    void Text::draw(const char* text) {
        ImGui::TextWrapped(text);
    }
}