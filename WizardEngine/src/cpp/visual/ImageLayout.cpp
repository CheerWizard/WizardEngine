//
// Created by mecha on 01.10.2021.
//

#include <visual/ImageLayout.h>
#include <imgui.h>

namespace engine::visual {

    void ImageLayout::onUpdate(time::Time dt) {
        if (!_isVisible) return;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0 , 0});

        // end updating if window can't be created!
        if (!ImGui::Begin(props.title, &_isVisible)) {
            end();
            return;
        }

        _isFocused = ImGui::IsWindowFocused();
        ENGINE_INFO("{0} is focused = {1}", props.title, _isFocused);

        ImVec2 imageSize = ImGui::GetContentRegionAvail();

        if (_isFocused && (imageSize.x != (float) props.width || imageSize.y != (float) props.height)) {
            // we will resize image content only if user stopped holding mouse!
            props.width = (uint32_t) imageSize.x;
            props.height = (uint32_t) imageSize.y;
            if (_callback != nullptr) {
                _callback->onImageResized(props.width, props.height);
            }
        }

        ImGui::Image((void*)id, imageSize, { 1, 1 }, { 0, 0 });
        onRender(dt);
        end();
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