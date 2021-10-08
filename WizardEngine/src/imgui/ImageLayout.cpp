//
// Created by mecha on 01.10.2021.
//

#include "ImageLayout.h"

#include <imgui.h>

namespace engine {

    void ImageLayout::onUpdate(Time dt) {
        if (!_isVisible) return;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0 , 0});

        static bool open = true;
        auto isClosed = !ImGui::Begin(_props.name, &open, ImGuiWindowFlags_AlwaysUseWindowPadding);

        if (isClosed && _isClosable) {
            _isVisible = false;
            end();
            return;
        }

        _isFocused = ImGui::IsWindowFocused();
        ENGINE_INFO("{0} is focused = {1}", _props.name, _isFocused);

        ImVec2 imageSize = ImGui::GetContentRegionAvail();

        if (!_isHoldingMouse && (imageSize.x != (float) _props.width || imageSize.y != (float) _props.height)) {
            _props.width = (uint32_t) imageSize.x;
            _props.height = (uint32_t) imageSize.y;
            if (_callback != nullptr) {
                _callback->onImageResized(_props.width, _props.height);
            }
        }

        ImGui::Image((void*) _image->getId(), imageSize);
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

    void ImageLayout::load(const std::string &fileName) {
        _image->recreate();
        _image->bind();
        _image->load(fileName);
    }

    void ImageLayout::end() {
        ImGui::End();
        ImGui::PopStyleVar();
    }

}