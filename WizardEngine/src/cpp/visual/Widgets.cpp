//
// Created by mecha on 01.09.2022.
//

#include <visual/Widgets.h>
#include <visual/Theme.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

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

    void Text::label(const char* text, const vec2f& padding) {
        // begin padding
        ImVec2 sz = ImGui::CalcTextSize(text);
        ImVec2 cursor = ImGui::GetCursorPos();
        ImVec2 paddingSize = { sz.x + padding.x() * 2, sz.y + padding.y() * 2 };
        ImGui::InvisibleButton("##padded-text", paddingSize);
        ImVec2 final_cursor_pos = ImGui::GetCursorPos();
        ImVec2 cursorPos = { cursor.x + padding.x(), cursor.y + padding.y() };
        ImGui::SetCursorPos(cursorPos);
        // draw text label
        ImGui::TextWrapped(text);
        // end padding
        ImGui::SetCursorPos(final_cursor_pos);
    }

    bool Text::field(const char* id, const char* title, std::string* text, const vec2f& padding, bool alignLeft) {
        // begin padding
        ImVec2 sz = ImGui::CalcTextSize(text->c_str());
        ImVec2 cursor = ImGui::GetCursorPos();
        ImVec2 paddingSize = { sz.x + padding.x() * 2, sz.y + padding.y() * 2 };
        ImGui::InvisibleButton("##padded-text", paddingSize);
        ImVec2 final_cursor_pos = ImGui::GetCursorPos();
        ImVec2 cursorPos = { cursor.x + padding.x(), cursor.y + padding.y() };
        ImGui::SetCursorPos(cursorPos);
        // draw text input
        bool result;
        if (alignLeft) {
            ImGui::Text(title);
            ImGui::SameLine();
            result = ImGui::InputText(id, text);
        } else {
            result = ImGui::InputText(title, text);
        }
        // end padding
        ImGui::SetCursorPos(final_cursor_pos);
        return result;
    }

    void Text::centered(const char *text, float maxWidth, int lineId, bool separator) {
        ImGui::Spacing();
        ImGui::SameLine((maxWidth / 2) - (ImGui::CalcTextSize(text).x / 2));
        ImGui::Text(text);
        ImGui::Spacing();
        if (separator) Line::draw(lineId);
    }

    bool Button::textButton(const char* text, const vec2f& size,
                            float paddingTop, float paddingBottom, float paddingLeft, float paddingRight) {
        if (paddingTop > 0) {
            ImGui::InvisibleButton("##padded-button-top", { size.x(), paddingTop });
        }
        if (paddingLeft > 0) {
            ImGui::InvisibleButton("##padded-button-left", { paddingLeft, size.y() });
            ImGui::SameLine();
        }
        bool result = ImGui::Button(text, { size.x(), size.y() });
        if (paddingRight > 0) {
            ImGui::SameLine();
            ImGui::InvisibleButton("##padded-button-right", { paddingRight, size.y() });
        }
        if (paddingBottom > 0) {
            ImGui::InvisibleButton("##padded-button-bottom", { size.x(), paddingBottom });
        }
        return result;
    }

    void Line::draw(int id) {
        std::string newId = "draw_line_" + std::to_string(id);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));

        ImGui::BeginChild(newId.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 1), false);
        ImGui::Separator();
        ImGui::EndChild();

        ImGui::PopStyleColor();
    }

    void CustomCheckbox::draw(BoolUniform &uniform) {
        draw(uniform.name, uniform.value);
    }

    void CustomCheckbox::draw(const char *label, bool &value) {
        bool result = false;
        auto* drawList = ImGui::GetWindowDrawList();

        const bool isMixedState = ImGuiItemFlags_MixedValue;

        const auto pos = ImGui::GetCursorScreenPos();
        const auto mousePos = ImGui::GetMousePos();

        const auto itemSpacing = ImGui::GetStyle().ItemSpacing;
        const float lineHeight = ImGui::GetTextLineHeight();
        const float boxSize = std::floor(lineHeight * 0.9f);
        const float boxOffsetHorz = std::ceil(itemSpacing.x * 1.3f);
        const float boxOffsetVert = itemSpacing.y + std::floor(0.5f * (lineHeight - boxSize));
        const float clearance = boxSize * 0.2f;
        const auto corner = ImVec2 { pos.x + boxOffsetHorz, pos.y + boxOffsetVert };

        char buf[1024];
        strcpy(buf, label);
        for (int i = 0; i < sizeof(buf); ++i)
        {
            if (buf[i] == '#')
            {
                buf[i] = '\0';
                break;
            }
        }
        const float labelWidth = ImGui::CalcTextSize(buf).x;

        bool isHovered = ImRect(pos, { pos.x + lineHeight + labelWidth + 2.0f * itemSpacing.x,
                                       pos.y + lineHeight }).Contains(mousePos);

        ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
        ImVec4 colorMark = color;
        color.w *= isHovered ? 1.0f : 0.25f;
        drawList->AddRect(corner, { corner.x + boxSize, corner.y + boxSize }, ImColor(color), 0.0f, 0, 1.0f);

        if (isHovered && ImGui::IsWindowHovered())
        {
            if (ImGui::IsMouseClicked(0))
            {
                if (isMixedState)
                {
                    value = false;
                }
                else
                {
                    value = !value;
                }
                result = true;
            }
        }

        if (isMixedState)
        {
            drawList->AddRectFilled(
                    { corner.x + clearance, corner.y + clearance},
                    { corner.x + boxSize - clearance, corner.y + boxSize - clearance},
                    ImColor(colorMark)
            );
        }
        else if (value) {
            ImVec2 checkMarkPts[3] = {
                    { corner.x + clearance, corner.y + clearance + boxSize * 0.3f },
                      { corner.x + boxSize * 0.5f, corner.y + boxSize - clearance },
                        { corner.x + boxSize - clearance, corner.y + clearance }
            };
            drawList->AddPolyline(checkMarkPts, 3, ImColor(colorMark), false, 2.5f);
        }

        ImGui::Dummy(ImVec2(lineHeight + itemSpacing.x, lineHeight));

        if (strlen(buf) > 0) {
            ImGui::SameLine();
            ImGui::AlignTextToFramePadding();
            ImGui::Text(buf);
        }

        ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x,
                                         pos.y + ImGui::GetTextLineHeightWithSpacing() + itemSpacing.y));

        value = result;
    }

    bool ColorPicker::draw(Vec4fUniform& colorUniform) {
        return ImGui::ColorPicker4(colorUniform.name, toFloatPtr(colorUniform));
    }
}