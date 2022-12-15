//
// Created by mecha on 07.10.2021.
//

#pragma once

#include <visual/ImageLayout.h>

#include <graphics/core/Renderer.h>
#include <graphics/core/io/ModelFile.h>
#include <graphics/camera/Camera.h>

#include <platform/graphics/FrameBuffer.h>

namespace engine::visual {

    using namespace engine::graphics;

    class ENGINE_API MeshLayout : public ImageLayout, public ImageLayoutCallback {

    public:
        MeshLayout(
                const ImageLayoutProps &props,
                const Ref<VIRenderer<io::ModelVertex>> &renderer,
                const Ref<FrameBuffer> &frame,
                const Ref<Camera3D> &camera3D
        );
        ~MeshLayout() override;

    public:
        void onRender(const time::Time &dt) override;

    public:
        inline const Ref<Camera3D>& getCamera3D() {
            return _camera3D;
        }

        inline void setEntity(const ecs::Entity& entity) {
            _entity = entity;
        }

        inline void setMesh(const BaseMeshComponent<io::ModelVertex> &meshComponent) {
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
        void rotateEntity(const time::Time &dt);

    private:
        Ref<VIRenderer<io::ModelVertex>> _renderer;
        Ref<FrameBuffer> _frame;
        Ref<Camera3D> _camera3D;
        ecs::Entity _entity;
        bool _shouldRotateEntity = true;

    };

}
