//
// Created by mecha on 07.10.2021.
//

#pragma once

#include <gui/ImageLayout.h>

#include <graphics/core/Renderer.h>
#include <graphics/camera/CameraController.h>
#include <graphics/core/io/ModelFile.h>

#include <platform/graphics/FrameBuffer.h>

namespace engine::gui {

    using namespace engine::graphics;

    class MeshLayout : public ImageLayout, public ImageLayoutCallback {

    public:
        MeshLayout(
                const ImageLayoutProps &props,
                const Ref<VIRenderer> &renderer,
                const Ref<FrameBuffer> &frame,
                const Ref<CameraController> &cameraController
        ) : ImageLayout(props),
        _renderer(renderer), _frame(frame), _cameraController(cameraController) {
            create();
        }

        ~MeshLayout() override {
            destroy();
        }

    public:
        void onRender(const time::Time &dt) override;

    public:
        inline const Ref<CameraController>& getCameraController() {
            return _cameraController;
        }

        inline void setEntity(const ecs::Entity& entity) {
            _entity = entity;
        }

        inline void setMesh(const io::ModelMeshComponent &meshComponent) {
            _entity.update<BaseMeshComponent<io::ModelVertex>>(meshComponent);
        }

        inline void setRotateEntity(const bool &enabled) {
            _shouldRotateEntity = enabled;
        }

    public:
        void onImageResized(const uint32_t &width, const uint32_t &height) override;

        void onKeyPressed(event::KeyCode keyCode);
        void onKeyHold(event::KeyCode keyCode);
        void onKeyReleased(event::KeyCode keyCode);
        void onKeyTyped(event::KeyCode keyCode);

    private:
        void create();
        void destroy();
        void rotateEntity(const time::Time &dt);

    private:
        Ref<VIRenderer> _renderer;
        Ref<FrameBuffer> _frame;
        Ref<CameraController> _cameraController;
        ecs::Entity _entity;
        bool _shouldRotateEntity = true;

    };

}
