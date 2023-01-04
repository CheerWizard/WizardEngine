//
// Created by mecha on 05.04.2022.
//

#include <core/application_test.h>

Application* engine::core::createApplication() {
    RUNTIME_INFO("Creating Test Application...");
    return new test::Application();
}

extern const char* engineLogName() {
    return "Engine";
}

extern const char* editorLogName() {
    return "Editor";
}

extern const char* runtimeLogName() {
    return "Test";
}

namespace test {

    void Application::onCreate() {
        engine::core::Application::onCreate();

        ThreadPoolScheduler->execute([]() {
            LocalAssetManager::readAll("assets/scenes");
        });

        KEY_PRESSED(D1) { [this](KeyCode keycode) {
            switchHDR();
        }};

        KEY_PRESSED(D2) { [this](KeyCode keycode) {
            switchBlur();
        }};

        KEY_PRESSED(D3) { [this](KeyCode keycode) {
            switchSharpen();
        }};

        KEY_PRESSED(D4) { [this](KeyCode keycode) {
            switchEdgeDetection();
        }};

        KEY_PRESSED(D5) { [this](KeyCode keycode) {
            switchBloom();
        }};

        KEY_PRESSED(D9) { [this](KeyCode keycode) {
            activeSceneFrame->setRenderTargetIndex(0);
        }};

        KEY_PRESSED(D0) { [this](KeyCode keycode) {
            activeSceneFrame->setRenderTargetIndex(1);
        }};

        KEY_PRESSED(M) { [this](KeyCode keycode) {
            switchMSAA();
        }};

//        KEY_PRESSED(KeyCode::D1, audio::MediaPlayer::pause(););
//        KEY_PRESSED(KeyCode::D2, audio::MediaPlayer::stop(););
//        KEY_PRESSED(KeyCode::D3, audio::MediaPlayer::playStream(););

//        KEY_PRESSED(
//                KeyCode::R,
//                // notify all players to rotate a survival pack
//                GDHeader header(CLIENT_TO_CLIENT, ROTATE_SURVIVAL_PACK);
//                GDFloat rotation(0.5f);
//                udp::Client::getRequestQueue().push(header, rotation);
//        );

//        KEY_PRESSED(
//                KeyCode::K,
//                io::RemoteAssetManager::saveScene(Application::get().activeScene);
//        );
//
//        KEY_PRESSED(
//                KeyCode::L,
//                io::RemoteAssetManager::loadScene(Application::get().activeScene->getName().c_str());
//        );
        enableMouseHovering = true;
        enableMouseCursor = true;
    }

    void Application::onVisualCreate() {
        m_SceneHierarchy = new SceneHierarchy(getNativeWindow(), this);
        m_Toolbar = new Toolbar();
    }

    void Application::onDestroy() {
        engine::core::Application::onDestroy();
        ThreadPoolScheduler->execute([]() {
            LocalAssetManager::writeAll("assets/scenes");
        });
        // we need to use render thread to free visual resources
        RenderScheduler->execute([this]() {
            AssetBrowser::destroy();
            delete m_SceneHierarchy;
            delete m_Toolbar;
        });
        // following visual tools can be cleared in any thread.
        VisualLog.clear();
        Console::get().clearLog();
        Console::get().clearHistory();
        ProjectsPanel::saveProjects();
    }

    void Application::onUpdate() {
        engine::core::Application::onUpdate();
        // notify all players to rotate a survival pack
//        GDHeader header(CLIENT_TO_CLIENT, ROTATE_SURVIVAL_PACK);
//        GDFloat rotation(0.005f);
//        udp::Client::get().getRequestQueue().push(header, rotation);
//        tcp::Client::get().getRequestQueue().push(header, rotation);

        activeScene->getSkybox().get<Skybox>()->rotate({0, 0.001f, 0});

//        auto hoveredTransform = Application::get().hoveredEntity.get<Transform3dComponent>();
//        if (hoveredTransform) {
//            ENGINE_INFO("hoveredEntity: transform");
//            hoveredTransform->modelMatrix.rotation[1] += 0.1f;
//            hoveredTransform->modelMatrix.apply();
//        }
    }

    void Application::onVisualDraw() {
        engine::core::Application::onVisualDraw();
        Visual::drawDockspace();
        // toolbar
        m_Toolbar->draw();
//        // Scene hierarchy and properties
        m_SceneHierarchy->onUpdate(dt);
//        // Scene view port
        sceneViewport.setRenderTarget(RenderSystem::finalRenderTargetId);
        sceneViewport.onUpdate(dt);
//        // Console
        Console::get().draw(1024, 768);
//        // Troubleshoot
        ProfilerMenu::draw(1024, 768);
//        // Projects storage
        ProjectsPanel::get().draw();
//        // Assets explorer
        AssetBrowser::draw(dt);
//        // Logger
        VisualLog.draw();
    }

    void Application::switchMSAA() {
        msaaEnabled = !msaaEnabled;
        setSampleSize(msaaEnabled ? 8 : 1);
    }

    void Application::switchHDR() {
        hdrEffect->enabled = !Application::get().hdrEffect->enabled;
    }

    void Application::switchBlur() {
        blurEffect->enabled = !Application::get().blurEffect->enabled;
    }

    void Application::switchSharpen() {
        sharpenEffect->enabled = !Application::get().sharpenEffect->enabled;
    }

    void Application::switchEdgeDetection() {
        edgeDetectionEffect->enabled = !Application::get().edgeDetectionEffect->enabled;
    }

    void Application::switchGaussianBlur() {
        gaussianBlurEffect->enabled = !Application::get().gaussianBlurEffect->enabled;
    }

    void Application::switchBloom() {
        switchHDR();
        switchGaussianBlur();
    }

    void Application::updateGizmo() {
//        if (EventRegistry::keyHold(KeyCode::LeftControl) && Input::isMousePressed(MouseCode::ButtonLeft)) {
//            Entity selectedEntity = Application::get().hoveredEntity;
//            auto* selectedTransform = selectedEntity.get<Transform3dComponent>();
//            if (selectedTransform) {
//                Application::get().selectedEntity = selectedEntity;
//                showGizmo = !showGizmo;
//            }
//        }
//
//        if (showGizmo) {
//            // get selected entity transform
//            auto& app = Application::get();
//            Entity selectedEntity = app.selectedEntity;
//            auto* selectedTransform = selectedEntity.get<Transform3dComponent>();
//            // get window position and size
//            vec2f windowSize = {
//                    static_cast<float>(app.getWindowWidth()),
//                    static_cast<float>(app.getWindowHeight())
//            };
//            int xPos = 0, yPos = 0;
//            app.getWindow()->getPosition(xPos, yPos);
//            // draw translation gizmo
//            Gizmo::drawTranslate(mainCamera, *selectedTransform,
//                                 { static_cast<float>(xPos), static_cast<float>(yPos) },
//                                 windowSize);
//        }
    }

    void Application::onSceneRemoved(const Ref<Scene> &scene) {
    }

    void Application::onEntityRemoved(const Entity &entity) {
    }

    void Application::onSceneSelected(const Ref<Scene> &scene) {
        setActiveScene(scene);
        sceneViewport.setScene(scene);
    }

    void Application::onEntitySelected(const Entity &entity) {

    }
}