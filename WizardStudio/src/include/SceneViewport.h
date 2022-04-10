//
// Created by mecha on 08.01.2022.
//

#pragma once

#include <gui/ImageLayout.h>

namespace studio {

    using namespace engine::gui;

    class DragDropCallback {
    public:
        virtual void onObjDragged(const std::string &fileName) = 0;
        virtual void onImageDragged(const std::string &fileName) = 0;
    };

    class SceneViewport : public ImageLayout {

    public:
        SceneViewport(const ImageLayoutProps& props) : ImageLayout(props) {}
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
        void onRender(const engine::time::Time &dt) override;

    private:
        DragDropCallback* dragDropCallback = nullptr;

    };

}
