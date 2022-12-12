//
// Created by mecha on 01.10.2021.
//

#include <visual/ImageLayout.h>
#include <visual/Visual.h>
#include <core/Application.h>
#include <imgui.h>

namespace engine::visual {

    void ImageLayout::onUpdate(time::Time dt) {
        ImGui::Begin("Viewport");

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();

        scene->viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        scene->viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        viewportFocused = ImGui::IsWindowFocused();
        viewportHovered = ImGui::IsWindowHovered();

        Visual::blockEvents = !viewportHovered;

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        scene->viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        ImGui::Image((ImTextureID)(id),
                     ImVec2{ scene->viewportSize.x, scene->viewportSize.y },
                     ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();
    }

    void ImageLayout::destroy() {
        removeCallback();
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