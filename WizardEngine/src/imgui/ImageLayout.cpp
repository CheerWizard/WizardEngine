//
// Created by mecha on 01.10.2021.
//

#include "ImageLayout.h"

#include <imgui.h>

namespace engine {

    void ImageLayout::onUpdate(Time deltaTime) {
        if (!_isVisible) return;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0 , 0});

        static bool* open = _isClosable ? &_isClosable : nullptr;

        ImGui::Begin(_props.name, open);

        ImVec2 imageSize = ImGui::GetContentRegionAvail();

        if (!_isHoldingMouse && (imageSize.x != (float) _props.width || imageSize.y != (float) _props.height)) {
            _props.width = (uint32_t) imageSize.x;
            _props.height = (uint32_t) imageSize.y;
            _callback->onImageResized(_props.width, _props.height);
        }

        ImGui::Image((void*) _image->getId(), imageSize);

        ImGui::End();
        ImGui::PopStyleVar();
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

}