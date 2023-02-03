//
// Created by mecha on 01.10.2021.
//

#include <visual/ImageLayout.h>
#include <visual/Visual.h>
#include <visual/FontAwesome4.h>
#include <core/Application.h>
#include <imgui.h>

namespace engine::visual {

    ImageLayout::~ImageLayout() {
        removeCallback();
    }

    void ImageLayout::onUpdate(time::Time dt) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        static bool open = true;
        ImGui::Begin(ICON_FA_TELEVISION" Viewport", &open);

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();

        bool viewportFocused = ImGui::IsWindowFocused();
        bool viewportHovered = ImGui::IsWindowHovered();
        Visual::blockEvents = !viewportHovered || !viewportFocused;

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