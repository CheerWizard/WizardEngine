//
// Created by mecha on 01.10.2021.
//

#include <imgui/ImageLayout.h>
#include <event/MouseCodes.h>
#include <imgui.h>

namespace engine {

    void ImageLayout::onUpdate(Time dt) {
        if (!_isVisible) return;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0 , 0});

        // end updating if window can't be created!
        if (!ImGui::Begin(_props.title, &_isVisible)) {
            end();
            return;
        }

        _isFocused = ImGui::IsWindowFocused();
        ENGINE_INFO("{0} is focused = {1}", _props.title, _isFocused);

        ImVec2 imageSize = ImGui::GetContentRegionAvail();

        if (_isFocused && (imageSize.x != (float) _props.width || imageSize.y != (float) _props.height)) {
            // we will resize image content only if user stopped holding mouse!
            _props.width = (uint32_t) imageSize.x;
            _props.height = (uint32_t) imageSize.y;
            if (_callback != nullptr) {
                _callback->onImageResized(_props.width, _props.height);
            }
        }

        ImGui::Image((void*)id, imageSize);
        onRender(dt);
        end();
    }

    void ImageLayout::destroy() {
        removeCallback();
    }

    void ImageLayout::onMousePressed(MouseCode mouseCode) {
        _isHoldingMouse = true;
    }

    void ImageLayout::onMouseRelease(MouseCode mouseCode) {
        _isHoldingMouse = false;
    }

    void ImageLayout::onMouseScrolled(double xOffset, double yOffset) {
    }

    void ImageLayout::end() {
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void ImageLayout::onRender(const Time &dt) {
        // do nothing here!
    }

}