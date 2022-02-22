//
// Created by mecha on 04.10.2021.
//

#include <FLApplication.h>
#include <FLLayer.h>
#include <Launcher.h>

engine::Application* engine::createApplication() {
    EDITOR_INFO("Creating FLApplication...");
    return new fairy::FLApplication();
}

extern const char* engineLogName() {
    return "Engine";
}

extern const char* editorLogName() {
    return "Editor";
}

extern const char* runtimeLogName() {
    return "Runtime";
}

namespace fairy {

    void FLApplication::onCreate() {
        Application::onCreate();
        EDITOR_INFO("Current workspace '{0}'", CURRENT_WORKING_DIR);

        auto* flLayer = new FLLayer(
                this,
                engine::ImGuiLayerProps {
                    getWindowWidth(),
                    getWindowHeight()
                }
        );

        pushBack(flLayer);

        setWindowIcon("FairyLab.png");
    }

    engine::WindowProps FLApplication::createWindowProps() {
        return engine::WindowProps {
            "Fairy Lab"
        };
    }

}