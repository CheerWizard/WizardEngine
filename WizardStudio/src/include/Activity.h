//
// Created by mecha on 04.10.2021.
//

#pragma once

#include <gui/ImguiLayer.h>
#include <gui/ImageLayout.h>
#include <gui/SceneHierarchy.h>
#include <gui/MeshLayout.h>

#include "AssetBrowser.h"
#include "SceneViewport.h"

namespace studio {

    using namespace engine::core;
    using namespace engine::gui;
    using namespace engine::event;

    class Activity : public ImGuiLayer, AssetBrowserCallback, SceneHierarchyCallback, DragDropCallback {

        class ImagePreviewCallback : public ImageLayoutCallback {

        public:
            ImagePreviewCallback(Activity &parent) : _parent(parent) {}
            ~ImagePreviewCallback() = default;

        public:
            void onImageResized(const uint32_t &width, const uint32_t &height) override;

        private:
            Activity& _parent;

        };

        class ScenePreviewCallback : public ImageLayoutCallback {

        public:
            ScenePreviewCallback(Activity &parent) : _parent(parent) {}
            ~ScenePreviewCallback() = default;

        public:
            void onImageResized(const uint32_t &width, const uint32_t &height) override;

        private:
            Activity& _parent;

        };

    public:
        Activity(Application* app, const ImGuiLayerProps &props) : ImGuiLayer(app, props) {
            create();
        }

        ~Activity() override {
            destroy();
        }

    public:
        void onPrepare() override;
        void onUpdate(engine::time::Time dt) override;

        void onMousePressed(MouseCode mouseCode) override;
        void onMouseRelease(MouseCode mouseCode) override;

        void onKeyPressed(KeyCode keyCode) override;
        void onKeyHold(KeyCode keyCode) override;
        void onKeyReleased(KeyCode keyCode) override;
        void onKeyTyped(KeyCode keyCode) override;

        void onWindowClosed() override;

    protected:
        void onRender(engine::time::Time dt) override;

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

        void onEntityRemoved(const Entity &entity) override;

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

        ImageLayout _texturePreview = ImageLayout({
            "Texture Preview",
            512,
            512
        });

        SceneHierarchy _sceneHierarchy = SceneHierarchy(app->activeScene);

        Ref<AssetBrowser> _assetBrowser;

        Ref<MeshLayout> _objPreview;

        ImagePreviewCallback* _imagePreviewCallback = nullptr;
        ScenePreviewCallback* _scenePreviewCallback = nullptr;

        Ref<Scene> editorScene = createRef<Scene>(); // store entities in scope of Editor and not Runtime!

        Ref<Camera3dController> activeSceneCameraController;

        bool windowClosePressed = false;

        Entity car;
    };

}