//
// Created by mecha on 04.10.2021.
//

#pragma once

#include <imgui/ImguiLayer.h>
#include <imgui/ImageLayout.h>
#include <imgui/SceneHierarchy.h>
#include <imgui/MeshLayout.h>

#include "AssetBrowser.h"
#include "SceneViewport.h"

namespace studio {

    class Activity : public engine::ImGuiLayer, AssetBrowserCallback, engine::SceneHierarchyCallback, DragDropCallback {

        class ImagePreviewCallback : public engine::ImageLayoutCallback {

        public:
            ImagePreviewCallback(Activity &parent) : _parent(parent) {}
            ~ImagePreviewCallback() = default;

        public:
            void onImageResized(const uint32_t &width, const uint32_t &height) override;

        private:
            Activity& _parent;

        };

        class ScenePreviewCallback : public engine::ImageLayoutCallback {

        public:
            ScenePreviewCallback(Activity &parent) : _parent(parent) {}
            ~ScenePreviewCallback() = default;

        public:
            void onImageResized(const uint32_t &width, const uint32_t &height) override;

        private:
            Activity& _parent;

        };

    public:
        Activity(engine::Application* app, const engine::ImGuiLayerProps &props) : engine::ImGuiLayer(app, props) {
            create();
        }

        ~Activity() override {
            destroy();
        }

    public:
        void onPrepare() override;
        void onUpdate(engine::Time dt) override;

        void onMousePressed(engine::MouseCode mouseCode) override;
        void onMouseRelease(engine::MouseCode mouseCode) override;

        void onKeyPressed(engine::KeyCode keyCode) override;
        void onKeyHold(engine::KeyCode keyCode) override;
        void onKeyReleased(engine::KeyCode keyCode) override;
        void onKeyTyped(engine::KeyCode keyCode) override;

        void onWindowClosed() override;

    protected:
        void onRender(engine::Time dt) override;

    private:
        void create();
        void createTest();
        void destroy();

        void createAssetBrowser();

        void onImageOpen(const std::string &fileName) override;
        void onObjOpen(const std::string &fileName) override;

        void onAssetImported(const std::string &assetPath) override;
        void onAssetExported(const std::string &assetPath) override;
        void onAssetRemoved(const std::string &assetPath) override;

        void onObjDragged(const std::string &fileName) override;

        void onImageDragged(const std::string &fileName) override;

        void onEntityRemoved(const engine::Entity &entity) override;

    private:
        SceneViewport sceneViewport = SceneViewport({
            "Scene",
                props.width,
                props.height
        });

        SceneViewport screenViewport = SceneViewport({
            "Screen",
            props.width,
            props.height
        });

        engine::ImageLayout _texturePreview = engine::ImageLayout({
            "Texture Preview",
            512,
            512
        });

        engine::SceneHierarchy _sceneHierarchy = engine::SceneHierarchy(app->activeScene);

        engine::Ref<AssetBrowser> _assetBrowser;

        engine::Ref<engine::MeshLayout> _objPreview;

        ImagePreviewCallback* _imagePreviewCallback = nullptr;
        ScenePreviewCallback* _scenePreviewCallback = nullptr;

        engine::Ref<engine::Scene> editorScene = engine::createRef<engine::Scene>(); // store entities in scope of Editor and not Runtime!

        engine::Camera3D activeSceneCamera;
        engine::Ref<engine::Camera3dController> activeSceneCameraController;

        bool windowClosePressed = false;

        Entity car;
    };

}