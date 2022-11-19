//
// Created by mecha on 01.10.2021.
//

#pragma once

#include <ecs/Scene.h>
#include <time/Time.h>
#include <event/MouseCodes.h>

namespace engine::visual {

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

        virtual ~ImageLayout() {
            destroy();
        }

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

        inline const bool& isFocused() const {
            return _isFocused;
        }

        inline void setId(const uint32_t &id) {
            this->id = id;
        }

    public:
        virtual void onMousePressed(event::MouseCode mouseCode);
        virtual void onMouseRelease(event::MouseCode mouseCode);
        virtual void onMouseScrolled(double xOffset, double yOffset);

    protected:
        void end();
        void destroy();

    public:
        ImageLayoutProps props;
        ImageLayoutCallback* _callback = nullptr;
        uint32_t id = 0;
        bool _isHoldingMouse = false;
        bool _isVisible = true;
        bool _isFocused = false;

    };

}