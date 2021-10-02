//
// Created by mecha on 01.10.2021.
//

#pragma once

#include "../ecs/Scene.h"
#include "../core/Layout.h"

namespace engine {

    struct SceneLayoutProps {
        const char* name;
        uint32_t width, height;
    };

    class SceneLayout : public Layout {

    public:
        SceneLayout(Scene* scene, const SceneLayoutProps &props) :
        _scene(scene),
        _props(props) {}

        ~SceneLayout() override = default;

    public:
        void onUpdate(Time deltaTime) override;

    private:
        Scene* _scene;
        SceneLayoutProps _props;

    };

}