#pragma once

#include <ecs/Scene.h>
#include <time/Time.h>
#include <event/MouseCodes.h>

namespace engine::visual {

    using namespace ecs;

    struct ENGINE_API ImageLayoutProps {
        const char* title;
        uint32_t width, height;

        ImageLayoutProps(const char* title, u32 width, u32 height) : title(title), width(width), height(height) {}
    };

    class ENGINE_API ImageLayoutCallback {
    public:
        virtual void onImageResized(const uint32_t &width, const uint32_t &height) = 0;
    };

    class ENGINE_API ImageLayout {

    public:
        ImageLayout(const ImageLayoutProps &props) : props(props) {}
        ImageLayout(const char* title, u32 width, u32 height) : props(title, width, height) {}
        virtual ~ImageLayout();

    public:
        virtual void onUpdate(time::Time dt);

    protected:
        virtual void onRender(const time::Time &dt);

    public:
        inline void setCallback(ImageLayoutCallback* imageLayoutCallback) {
            _callback = imageLayoutCallback;
        }

        inline void removeCallback() {
            _callback = nullptr;
        }

        inline void show() {
            _isVisible = true;
        }

        inline void hide() {
            _isVisible = false;
        }

        [[nodiscard]] inline bool isFocused() const {
            return _isFocused;
        }

        inline void setRenderTarget(u32 id) {
            this->id = id;
        }

    public:
        virtual void onMouseScrolled(double xOffset, double yOffset);

        inline void setScene(const Ref<Scene>& actualScene) {
            scene = actualScene;
        }

    protected:
        void end();

    private:
        Ref<Scene> scene = nullptr;
        ImageLayoutProps props;
        ImageLayoutCallback* _callback = nullptr;
        u32 id = 0;
        bool _isVisible = true;
        bool _isFocused = false;
    };

}