//
// Created by mecha on 01.10.2021.
//

#pragma once

#include <ecs/Scene.h>
#include <core/Time.h>
#include <core/MouseCodes.h>

namespace engine {

    struct ImageLayoutProps {
        const char* title;
        uint32_t width, height;
    };

    class ImageLayoutCallback {
    public:
        virtual void onImageResized(const uint32_t &width, const uint32_t &height) = 0;
    };

    class ImageLayout {

    public:
        ImageLayout(const ImageLayoutProps &props) : _props(props) {}

        virtual ~ImageLayout() {
            destroy();
        }

    public:
        virtual void onUpdate(Time dt);

    protected:
        virtual void onRender(const Time &dt);

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
        virtual void onMousePressed(MouseCode mouseCode);
        virtual void onMouseRelease(MouseCode mouseCode);
        virtual void onMouseScrolled(double xOffset, double yOffset);

    protected:
        void end();
        void destroy();

    protected:
        ImageLayoutProps _props;
        ImageLayoutCallback* _callback = nullptr;
        uint32_t id = 0;
        bool _isHoldingMouse = false;
        bool _isVisible = true;
        bool _isFocused = false;

    };

}