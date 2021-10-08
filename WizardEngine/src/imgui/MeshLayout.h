//
// Created by mecha on 07.10.2021.
//

#pragma once

#include "ImageLayout.h"
#include "../graphics/render/Renderer.h"
#include "../graphics/geometry/MeshComponent.h"

namespace engine {

    class MeshLayout : public ImageLayout, public ImageLayoutCallback, public KeyboardCallback {

    public:
        MeshLayout(const ImageLayoutProps &props, const Ref<Renderer> &renderer) :
        ImageLayout(props, renderer->getTextureBuffer()), _renderer(renderer) {
            create();
        }

        ~MeshLayout() override {
            destroy();
        }

    public:
        void onUpdate(Time dt) override;

    public:
        inline void setMesh(const Ref<MeshComponent> &meshComponent) {
            _meshComponent = meshComponent;
        }

        inline void setTransform(const Ref<TransformComponent3d> &transformComponent) {
            _transformComponent = transformComponent;
        }

        inline const Ref<Camera3dController>& getCameraController() {
            return _cameraController;
        }

    public:
        void setCameraController(const Ref<Camera3dController> &camera3DController);

        void onImageResized(const uint32_t &width, const uint32_t &height) override;

        void onKeyPressed(KeyCode keyCode) override;
        void onKeyHold(KeyCode keyCode) override;
        void onKeyReleased(KeyCode keyCode) override;
        void onKeyTyped(KeyCode keyCode) override;

    private:
        void create();
        void destroy();

    private:
        Ref<Renderer> _renderer;
        Ref<MeshComponent> _meshComponent = nullptr;
        Ref<TransformComponent3d> _transformComponent = nullptr;
        Ref<Camera3dController> _cameraController = nullptr;

    };

}
