//
// Created by mecha on 08.01.2022.
//

#pragma once

#include <imgui/ImageLayout.h>

namespace fairy {

    class DragDropCallback {
    public:
        virtual void onObjDragged(const std::string &fileName) = 0;
        virtual void onImageDragged(const std::string &fileName) = 0;
    };

    class SceneViewport : public engine::ImageLayout {

    public:
        SceneViewport(const engine::ImageLayoutProps& props) : engine::ImageLayout(props) {}
        ~SceneViewport() override {
            destroy();
            removeDragDropCallback();
        }

    public:
        inline void setDragDropCallback(DragDropCallback* dragDropCallback) {
            this->dragDropCallback = dragDropCallback;
        }

        inline void removeDragDropCallback() {
            dragDropCallback = nullptr;
        }

    public:
        void onRender(const engine::Time &dt) override;

    private:
        DragDropCallback* dragDropCallback = nullptr;

    };

}
