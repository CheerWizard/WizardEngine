//
// Created by mecha on 29.08.2021.
//

#include "events/Event.h"
#include "Memory.h"

namespace engine {

    struct WindowProps {
        std::string title;
        uint32_t width;
        uint32_t height;
        bool vSyncEnabled;

        explicit WindowProps(
                const std::string& title = "Wizard Engine",
                uint32_t width = 800,
                uint32_t height = 600,
                bool vSyncEnabled = false)
        : title(title), width(width), height(height), vSyncEnabled(vSyncEnabled) {

        }

    };

    class Window {

    public:
        explicit Window(const WindowProps& windowProps) : windowProps(windowProps) {

        };
        virtual ~Window() = default;

    public:
        virtual void onUpdate() = 0;
        virtual void* getNativeWindow() const = 0;

    public:

        virtual void enableVSync() {
            windowProps.vSyncEnabled = true;
        }

        virtual void disableVSync() {
            windowProps.vSyncEnabled = false;
        }

    public:

        inline uint32_t getWidth() const {
            return windowProps.width;
        }

        inline uint32_t getHeight() const {
            return windowProps.height;
        }

        inline bool isVSyncEnabled() const {
            return windowProps.vSyncEnabled;
        }

        inline void setEventCallback(EventCallback *callback) {
            eventCallback = callback;
        }

    public:
        static Scope<Window> create(const WindowProps& props = WindowProps());

    protected:
        void onEvent(Event& event) {
            if (eventCallback != nullptr) {
                eventCallback->onEvent(event);
            }
        }

    protected:
        WindowProps windowProps;
        EventCallback* eventCallback = nullptr;
        bool isInitialized = false;

    };


}