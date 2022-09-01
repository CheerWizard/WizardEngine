//
// Created by mecha on 01.09.2022.
//

#pragma once

#include <imgui.h>
#include <core/primitives.h>
#include <core/vector.h>
#include <event/Events.h>
#include <graphics/core/shader/Uniform.h>

namespace engine::visual {

    using namespace core;
    using namespace shader;
    using namespace math;

    struct VisualProps {
        f32 width = 0, height = 0;
    };

    class Visual final {

    public:
        // core lifecycle
        static void init(void* nativeWindow, const VisualProps& props);
        static void begin();
        static void end();
        static void release();
        // docking space
        static void beginDockspace(const char* name);
        static void setDockspace();
        static void endDockspace();
        // fonts
        static u32 addFont(const char* filepath, f32 fontSize);
        static void setDefaultFont(u32 fontIndex);
        static void setDefaultFont(const char* filepath, f32 fontSize);
        // theme
        static void setTheme();
        // events
        static void onKeyPressed(event::KeyCode keyCode);
        static void onKeyHold(event::KeyCode keyCode);
        static void onKeyReleased(event::KeyCode keyCode);
        static void onKeyTyped(event::KeyCode keyCode);
        static void onMouseScrolled(double xOffset, double yOffset);
        static void onMousePressed(event::MouseCode mouseCode);
        static void onMouseRelease(event::MouseCode mouseCode);
        static void onCursorMoved(double xPos, double yPos);
        static void onWindowClosed();
        static void onWindowResized(const uint32_t &width, const uint32_t &height);

    public:
        static VisualProps props;
        static vector<ImFont*> fonts;
        static bool fullScreen, openDockspace;
        static int windowFlags, dockspaceFlags;

    };

    class Panel final {

    public:
        static void begin(const char* title, const vec2f& size);
        static void end();

        static bool isFocused(ImGuiFocusedFlags flags = 0);
    };

}