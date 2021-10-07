//
// Created by mecha on 01.10.2021.
//

#pragma once

#include "../ecs/Scene.h"
#include "../core/Layout.h"
#include "../core/Events.h"

#include "../graphics/buffers/TextureBuffer.h"

namespace engine {

    struct ImageLayoutProps {
        const char* name;
        uint32_t width, height;
    };

    class ImageLayoutCallback {
    public:
        virtual void onImageResized(const uint32_t &width, const uint32_t &height) = 0;
    };

    class ImageLayout : public Layout, MouseCallback {

    public:
        ImageLayout(const ImageLayoutProps &props, const Ref<TextureBuffer>& image) :
                _props(props), _image(image) {}

        ~ImageLayout() {
            destroy();
        }

    public:
        void onUpdate(Time deltaTime) override;
        void load(const std::string &fileName);

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

        inline void setClosable(const bool &isClosable) {
            _isClosable = isClosable;
        }

        inline void setTextureId(const uint32_t &textureId) {
            _image->setId(textureId);
        }

    public:
        void onMousePressed(MouseCode mouseCode) override;
        void onMouseRelease(MouseCode mouseCode) override;
        void onMouseScrolled(double xOffset, double yOffset) override;

    private:
        void destroy();

    private:
        ImageLayoutProps _props;
        ImageLayoutCallback* _callback = nullptr;
        bool _isHoldingMouse = false;
        bool _isVisible = true;
        bool _isClosable = false;
        Ref<TextureBuffer> _image;

    };

}