//
// Created by mecha on 01.10.2021.
//

#include <visual/ImageLayout.h>
#include <visual/Visual.h>
#include <core/Application.h>
#include <imgui.h>

namespace engine::visual {

    ImageLayout::~ImageLayout() {
        removeCallback();
    }

    void ImageLayout::onUpdate(time::Time dt) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::Begin("Viewport");

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();

        viewportFocused = ImGui::IsWindowFocused();
        viewportHovered = ImGui::IsWindowHovered();
        Visual::blockEvents = !viewportHovered;

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        if (scene) {
            scene->viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            scene->viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
            scene->viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        }

        ImGui::Image(
                (ImTextureID)(id),
                viewportPanelSize,
                { 0, 1 },
                { 1, 0 }
        );

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void ImageLayout::onMousePressed(event::MouseCode mouseCode) {
        _isHoldingMouse = true;
    }

    void ImageLayout::onMouseRelease(event::MouseCode mouseCode) {
        _isHoldingMouse = false;
    }

    void ImageLayout::onMouseScrolled(double xOffset, double yOffset) {
    }

    void ImageLayout::end() {
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void ImageLayout::onRender(const time::Time &dt) {
        // do nothing here!
    }

}