//
// Created by mecha on 17.11.2021.
//

#pragma once

#include "../../../platform/includes/graphics/commands.h"

namespace engine {

    struct CullingComponent {
        bool enabled = false;
    };

    enum PolygonMode : uint8_t {
        VERTEX = 0,
        LINE = 1,
        FILL = 2
    };

    // used to execute direct graphics calls!
    class RenderCommands final {

    private:
        RenderCommands() = default;
        ~RenderCommands() = default;

    public:
        static void toggleCulling();
        static void setCulling(const CullingComponent &culling);
        static void trySetPolygonMode(const PolygonMode &polygonMode);
        static void logApiInfo();

    private:
        static PolygonMode activePolygonMode;
        static bool isCullingEnabled;

    };

}