//
// Created by mecha on 04.10.2021.
//

#include "FLApplication.h"
#include "FLLayer.h"
#include "Launcher.h"

engine::Application* engine::createApplication() {
    CLIENT_INFO("Creating FLApplication...");
    return new fairy::FLApplication();
}

namespace fairy {

    void FLApplication::onCreate() {
        Application::onCreate();
        CLIENT_INFO("Current workspace '{0}'", CURRENT_WORKING_DIR);

        auto* flLayer = new FLLayer(
                this,
                engine::ImGuiLayerProps {
                    getWindowWidth(),
                    getWindowHeight()
                }
        );

        pushBack(flLayer);
    }

    engine::WindowProps FLApplication::createWindowProps() {
        return engine::WindowProps {
            "Fairy Lab"
        };
    }

}