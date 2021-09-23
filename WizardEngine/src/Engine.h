//
// Created by mecha on 28.08.2021.
//

#pragma once

// export core systems.
#include "core/Application.h"
#include "core/Logger.h"
#include "core/Layer.h"
#include "core/Input.h"
#include "core/Layout.h"
#include "core/Time.h"
#include "core/Memory.h"

// export Graphics systems.
#include "graphics/GraphicsObject.h"

// export ImGui system.
#include "imgui/DemoLayout.h"

// export vendor libs.
#include "../vendor/imgui/imgui/imgui.h"

#include "core/TreeCache.h"
#include "graphics/buffers/VertexArray.h"
#include "graphics/buffers/VertexBuffer.h"
#include "graphics/render/Renderer.h"

#include "math/ViewProjectionMatrix.h"
#include "math/TransformMatrix.h"

extern engine::Application* engine::createApplication();

namespace engine {

    static void run() {
        INIT_CLIENT_LOG("Client");
        INIT_ENGINE_LOG("Engine");

        ENGINE_INFO("Creating Application...");
        auto app = createApplication();
        ENGINE_INFO("Application created!");

        ENGINE_INFO("Running Application...");
        app->run();

        ENGINE_INFO("Application terminated!");
        delete app;
    }

}