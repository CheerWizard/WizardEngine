//
// Created by mecha on 01.10.2021.
//

#pragma once

#include "../ecs/Scene.h"
#include "../core/Layout.h"

namespace engine {

    struct ImageLayoutProps {
        const char* name;
        uint32_t width, height, textureId;
    };

    class ImageLayout : public Layout {

    public:
        ImageLayout(const ImageLayoutProps &props) :
                _props(props) {}

        ~ImageLayout() = default;

    public:
        void onUpdate(Time deltaTime) override;

    public:
        inline void setTextureId(const uint32_t &textureId) {
            _props.textureId = textureId;
        }

        inline const uint32_t& getTextureId() const {
            return _props.textureId;
        }

    private:
        ImageLayoutProps _props;

    };

}