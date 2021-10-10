//
// Created by mecha on 04.10.2021.
//

#pragma once

#include "imgui/ImguiLayer.h"
#include "imgui/ImageLayout.h"
#include "imgui/SceneHierarchy.h"
#include "imgui/MeshLayout.h"
#include "imgui/FileEditor.h"

#include "AssetBrowser.h"

namespace fairy {

    class FLLayer : public engine::ImGuiLayer, AssetBrowserCallback {

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

        void onKeyHold(engine::KeyCode keyCode) override;

        void onKeyReleased(engine::KeyCode keyCode) override;

        void onKeyTyped(engine::KeyCode keyCode) override;

    protected:
        void onRender(engine::Time dt) override;

    private:
        void create();
        void destroy();

        void onPngOpen(const std::string &fileName) override;
        void onJpgOpen(const std::string &fileName) override;
        void onObjOpen(const std::string &fileName) override;
        void onGlslOpen(const std::string &filePath, const std::string &fileName) override;

        void onAssetImported(const std::string &assetPath) override;
        void onAssetExported(const std::string &assetPath) override;

        void onAssetRemoved(const std::string &assetPath) override;

    private:
        engine::ImageLayout _scenePreview = engine::ImageLayout({
            "Scene Preview",
                props.width,
                props.height
                },
        app->getGraphicsFactory()->newTextureBuffer()
        );

        engine::SceneHierarchy _sceneHierarchy = engine::SceneHierarchy(app->activeScene);

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
                app->getGraphicsFactory()->newTextureBuffer(),
                app->createFileDialog()
        );

        engine::Ref<engine::MeshLayout> _objPreview;

        engine::FileEditor _fileEditor = engine::FileEditor {};

        engine::Entity _humanEntity;

        ImagePreviewCallback* _imagePreviewCallback = nullptr;
        ScenePreviewCallback* _scenePreviewCallback = nullptr;

    };

}