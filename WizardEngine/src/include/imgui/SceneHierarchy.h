//
// Created by mecha on 03.10.2021.
//

#pragma once

#include <core/Time.h>
#include <core/String.h>
#include <ecs/Scene.h>

namespace engine {

    struct SceneHierarchyProps {
        const char* name = "Scene Hierarchy";
    };

    class SceneHierarchyCallback {
    public:
        virtual ~SceneHierarchyCallback() = default;

    public:
        virtual void onEntityRemoved(const Entity &entity) = 0;
    };

    class SceneHierarchy {

    public:
        SceneHierarchy(const SceneHierarchyProps &props = SceneHierarchyProps()) : _props(props) {}
        SceneHierarchy(const Ref<Scene>& scene, const SceneHierarchyProps &props = SceneHierarchyProps()) :
        _props(props), _scene(scene) {}

        ~SceneHierarchy() {
            destroy();
        }

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

        inline void setCallback(SceneHierarchyCallback* callback) {
            _callback = callback;
        }

        inline void removeCallback() {
            _callback = nullptr;
        }

    private:
        void draw(entt::registry& registry, bool instancingEnabled);
        void drawEntityNode(Entity &entity);
        void drawComponents(Entity &entity);

    private:
        void destroy();

    private:
        SceneHierarchyProps _props;
        Ref<Scene> _scene;
        Entity _selectedEntity;
        SceneHierarchyCallback* _callback = nullptr;

    };

}