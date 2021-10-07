//
// Created by mecha on 04.10.2021.
//

#pragma once

#include "imgui/ImguiLayer.h"
#include "imgui/ImageLayout.h"
#include "imgui/EntityLayout.h"

#include "AssetBrowser.h"

namespace fairy {

    class FLLayer : public engine::ImGuiLayer, engine::EntityLayoutCallback, AssetBrowserCallback {

        class ImagePreviewCallback : public engine::ImageLayoutCallback {

        public:
            ImagePreviewCallback(FLLayer &parent) : _parent(parent) {}
            ~ImagePreviewCallback() = default;

        public:
            void onImageResized(const uint32_t &width, const uint32_t &height) override;

        private:
            FLLayer& _parent;

        };

        class ScenePreviewCallback : public engine::ImageLayoutCallback {

        public:
            ScenePreviewCallback(FLLayer &parent) : _parent(parent) {}
            ~ScenePreviewCallback() = default;

        public:
            void onImageResized(const uint32_t &width, const uint32_t &height) override;

        private:
            FLLayer& _parent;

        };

    public:
        FLLayer(engine::Application* app, const engine::ImGuiLayerProps &props) : engine::ImGuiLayer(app, props) {
            create();
        }

        ~FLLayer() override {
            destroy();
        }

    public:
        void onPrepare() override;
        void onUpdate(engine::Time dt) override;

        void onKeyPressed(engine::KeyCode keyCode) override;
        void onMousePressed(engine::MouseCode mouseCode) override;
        void onMouseRelease(engine::MouseCode mouseCode) override;

    protected:
        void onRender(engine::Time dt) override;

    private:
        void create();
        void destroy();

        engine::Entity createEntity(const std::string &entityName) override;
        void removeEntity(engine::Entity entity) override;

        void onFileOpen(const std::string &fileName) override;

    private:
        engine::ImageLayout _scenePanel = engine::ImageLayout({
            "Scene Preview",
                props.width,
                props.height
                },
          app->getGraphicsFactory()->newTextureBuffer()
        );

        engine::EntityLayout _entityPanel = engine::EntityLayout({
            "Scene Hierarchy"
        });

        engine::ImageLayout _imagePreview = engine::ImageLayout {
            {"Image Preview"},
            app->getGraphicsFactory()->newTextureBuffer()
        };

        engine::Ref<AssetBrowser> _assetBrowser = engine::createRef<AssetBrowser>(
                AssetBrowserProps {
                    "Asset Browser",
                    CLIENT_ASSET_PATH,
                    },
                app->getGraphicsFactory()->newTextureBuffer(),
                app->getGraphicsFactory()->newTextureBuffer()
        );

        engine::Entity _carEntity;

        ImagePreviewCallback* _imagePreviewCallback = nullptr;
        ScenePreviewCallback* _scenePreviewCallback = nullptr;

    };

}