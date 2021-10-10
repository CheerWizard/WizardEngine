//
// Created by mecha on 03.10.2021.
//

#pragma once

#include "../core/Time.h"
#include "../core/String.h"
#include "../ecs/Scene.h"
#include "../ecs/Entity.h"

namespace engine {

    struct SceneHierarchyProps {
        const char* name = "Scene Hierarchy";
    };

    class SceneHierarchy {

    public:
        SceneHierarchy(const SceneHierarchyProps &props = SceneHierarchyProps()) : _props(props) {}
        SceneHierarchy(const Ref<Scene>& scene, const SceneHierarchyProps &props = SceneHierarchyProps()) :
        _props(props), _scene(scene) {}

    public:
        void onUpdate(Time dt);

    public:
        inline void setScene(const Ref<Scene> &scene) {
            _scene = scene;
        }

        inline const Entity& getSelectedEntity() const {
            return _selectedEntity;
        }

        inline void setSelectedEntity(const Entity& selectedEntity) {
            _selectedEntity = selectedEntity;
        }

    private:
        void drawEntityNode(Entity &entity);
        void drawComponents(Entity &entity);

    private:
        SceneHierarchyProps _props;
        Ref<Scene> _scene;
        Entity _selectedEntity;

    };

}