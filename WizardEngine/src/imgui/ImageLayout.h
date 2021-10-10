//
// Created by mecha on 01.10.2021.
//

#pragma once

#include "../ecs/Scene.h"
#include "../core/Events.h"
#include "../core/Time.h"

#include "../graphics/buffers/TextureBuffer.h"

namespace engine {

    struct ImageLayoutProps {
        const char* title;
        uint32_t width, height;
    };

    class ImageLayoutCallback {
    public:
        virtual void onImageResized(const uint32_t &width, const uint32_t &height) = 0;
    };

    class ImageLayout : public MouseCallback {

    public:
        ImageLayout(const ImageLayoutProps &props, const Ref<TextureBuffer>& image) :
                _props(props), _image(image) {}

        virtual ~ImageLayout() {
            destroy();
        }

    public:
        virtual void onUpdate(Time dt);
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

        inline const bool& isFocused() const {
            return _isFocused;
        }

    public:
        void onMousePressed(MouseCode mouseCode) override;
        void onMouseRelease(MouseCode mouseCode) override;
        void onMouseScrolled(double xOffset, double yOffset) override;

    protected:
        void end();

    private:
        void destroy();

    protected:
        ImageLayoutProps _props;
        ImageLayoutCallback* _callback = nullptr;
        Ref<TextureBuffer> _image;
        bool _isHoldingMouse = false;
        bool _isVisible = true;
        bool _isClosable = true;
        bool _isFocused = false;

    };

}