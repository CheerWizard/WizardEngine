//
// Created by mecha on 03.09.2022.
//

#include <visual/Troubleshoot.h>
#include <visual/Visual.h>
#include <visual/Widgets.h>
#include <profiler/Profiler.h>
#include <imgui.h>

namespace engine::visual {

    void ProfilerMenu::draw(const char *title, const vec2f& size) {
        Panel::begin(title, size);

        Slider::draw("FrameCount", profiler::Profiler::getExpectedFrames(), { 0, 1000 });
        bool& enableProfiler = profiler::Profiler::isEnabled();
        Checkbox::draw("enableProfiler", enableProfiler);
        ImGui::SameLine();
        if (enableProfiler) {
            enableProfiler = !ImGui::Button("Stop", { 80, 20 });
        } else {
            enableProfiler = ImGui::Button("Start", { 80, 20 });
        }

        Panel::end();
    }

}