//
// Created by mecha on 01.10.2021.
//

#pragma once

#include "../ecs/Scene.h"
#include "../core/Layout.h"

namespace engine {

    class ScenePanel : public Layout {

    public:
        ScenePanel(Scene* scene, const uint32_t &width, const uint32_t &height) :
        Layout(),
        _scene(scene),
        _height(height),
        _width(width) {}

        ~ScenePanel() = default;

    public:
        void onUpdate(Time deltaTime) override;

    private:
        Scene* _scene;
        uint32_t _width, _height;

    };

}