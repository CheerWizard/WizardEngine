//
// Created by mecha on 05.04.2022.
//

#pragma once

#include <core/Application.h>
#include <Launcher.h>

namespace test {

    class Application : public engine::core::Application, SceneHierarchyCallback {

    protected:
        void onCreate() override;
        void onVisualCreate() override;
        void onUpdate() override;
        void onVisualDraw() override;
        void onDestroy() override;

    public:
        void onSceneRemoved(const Ref<Scene> &scene) override;
        void onEntityRemoved(const Entity &entity) override;
        void onSceneSelected(const Ref<Scene> &scene) override;
        void onEntitySelected(const Entity &entity) override;

    private:
        void switchMSAA();
        void switchHDR();
        void switchBlur();
        void switchSharpen();
        void switchEdgeDetection();
        void switchGaussianBlur();
        void switchBloom();

        void updateGizmo();

    private:
        bool msaaEnabled = false;
        bool showGizmo = false;
        ImageLayout sceneViewport {
                "Scene",
                512,
                512
        };
        SceneHierarchy* m_SceneHierarchy;
        Toolbar* m_Toolbar;
    };

}
