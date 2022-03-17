//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "Entity.h"

namespace engine {

    class Scene : public EntityContainer {

    public:
        Scene() = default;
        ~Scene() {
            clear();
        }

    public:
        inline const uint32_t& getTextureId() const {
            return _textureId;
        }

        inline void setTextureId(const uint32_t &textureId) {
            _textureId = textureId;
        }

    private:
        uint32_t _textureId = 0; // id of texture that stores pixels of this scene
    };

    Ref<Scene> copy(Ref<Scene>& scene);

}