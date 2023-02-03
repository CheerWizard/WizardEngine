#pragma once

#include <imgui.h>

#include <core/primitives.h>
#include <core/vector.h>

#include <event/Events.h>

#include <graphics/core/shader/Uniform.h>
#include <graphics/camera/Camera.h>
#include <graphics/transform/TransformComponents.h>

#include <platform/tools/FileDialog.h>

#include <time/Time.h>

namespace engine::visual {

    using namespace core;
    using namespace shader;
    using namespace math;
    using namespace time;
    using namespace tools;

    class ENGINE_API Visual final {

    public:
        static void onUpdate(Time dt);
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
        static void onWindowResized(u32 width, u32 height);

        static void sameLine();
        static void separator();
        static void drawDockspace();

        static bool blocksKeyboard();
        static bool blocksMouse();
        static bool blocksTextInput();
        static bool blocksMousePos();

    public:
        static bool fullScreen;
        static bool openDockspace;
        static bool blockEvents;

    private:
        static vector<ImFont*> fonts;
        static int windowFlags;
        static int dockspaceFlags;
        static Ref<FileDialog> s_FileDialog;
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