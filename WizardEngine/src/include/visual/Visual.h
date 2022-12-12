//
// Created by mecha on 01.09.2022.
//

#pragma once

#include <imgui.h>
#include <core/primitives.h>
#include <core/vector.h>
#include <event/Events.h>
#include <graphics/core/shader/Uniform.h>
#include <graphics/camera/Camera.h>
#include <graphics/transform/TransformComponents.h>

namespace engine::visual {

    using namespace core;
    using namespace shader;
    using namespace math;

    class ENGINE_API Visual final {

    public:
        // core lifecycle
        static void init(void* nativeWindow);
        static void begin();
        static void end();
        static void release();
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

        static void sameLine();
        static void separator();
        static void showDemoWindow();

    public:
        static bool fullScreen;
        static bool openDockspace;
        static bool blockEvents;

    private:
        // docking space
        static void beginDockspace();
        static void setDockspace();
        static void endDockspace();

        static vector<ImFont*> fonts;
        static int windowFlags;
        static int dockspaceFlags;
    };

    using namespace graphics;

    class ENGINE_API Gizmo final {
    public:
        static void drawTranslate(Camera3D& camera, Transform3dComponent& transform,
                                  const vec2f& windowPosition,
                                  const vec2f& windowSize);
        static void drawRotate(Camera3D& camera, Transform3dComponent& transform,
                               const vec2f& windowPosition, const vec2f& windowSize);
        static void drawScale(Camera3D& camera, Transform3dComponent& transform,
                              const vec2f& windowPosition, const vec2f& windowSize);
        static void enable(bool enabled);
    };

    class ENGINE_API Panel final {
    public:
        static void begin(const char* title, const vec2f& size);
        static void end();
        static bool isFocused(ImGuiFocusedFlags flags = 0);
    };

}