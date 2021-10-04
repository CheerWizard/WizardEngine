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
        scene(scene),
        props(props) {}

        ~SceneLayout() override {
            destroy();
        }

    public:
        void onUpdate(Time deltaTime) override;

    private:
        void destroy();

    protected:
        Scene* scene;
        SceneLayoutProps props;

    };

}