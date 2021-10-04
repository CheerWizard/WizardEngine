//
// Created by mecha on 04.10.2021.
//

#pragma once

#include "../../WizardEngine/src/Engine.h"

namespace fl {

    class FLLayer : public engine::ImGuiLayer {

    public:
        FLLayer(void* nativeWindow,
                const uint32_t &width,
                const uint32_t &height,
                const char* tag = "FLLayer") : engine::ImGuiLayer(nativeWindow, width, height, tag) {}

        ~FLLayer() override = default;

    public:
        void onUpdate(engine::Time deltaTime) override;

    protected:
        void onCreate() override;
        void onDestroy() override;

    };

    class FLApplication : public engine::Application {

    protected:
        void onCreate() override;
        void onPrepare() override;
        void onDestroy() override;
        void onUpdate() override;
        void onKeyPressed(engine::KeyCode keyCode) override;

    private:
        engine::Entity carEntity;

    };

}