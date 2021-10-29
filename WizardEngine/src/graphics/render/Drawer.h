//
// Created by mecha on 27.09.2021.
//

#pragma once

#include <cstdint>
#include <string>

namespace engine {

    struct CullingComponent {
        bool enabled = false;
    };

    enum PolygonMode : uint8_t {
        POINT = 0,
        LINE = 1,
        FILL = 2
    };

    // interface to implement for specific platform.
    // used to make a GPU calls and control the screen.
    class Drawer {

    public:
        virtual ~Drawer() = default;

    public:
        void toggleCulling();
        void setCulling(const CullingComponent &culling);
        void setPolygonMode(const PolygonMode &polygonMode);

    public:
        virtual void drawQuadsIndices(const uint32_t &indexCount) = 0;
        virtual void drawElements(const uint32_t &indexCount) = 0;
        virtual void drawElements(const uint32_t &indexCount, const uint32_t &instanceCount) = 0;

        virtual void enableCulling() = 0;
        virtual void disableCulling() = 0;

        virtual void setPolygonMode() = 0;

    protected:
        PolygonMode activePolygonMode = FILL;
        bool isCullingEnabled = false;

    };

}