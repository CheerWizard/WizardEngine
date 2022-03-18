//
// Created by mecha on 08.01.2022.
//

#pragma once

#include <ecs/Entity.h>
#include "core/geometry/MeshComponent.h"
#include "transform/TransformComponents.h"

namespace engine {

    template<typename T>
    class Object3d : public Entity {

    public:
        Object3d(
                const std::string &tag,
                EntityContainer* container,
                const bool& instancingEnabled = false
        ) : Entity(tag, container, instancingEnabled) {
            add<Transform3dComponent>(transform3d());
            add<BaseMeshComponent<T>>();
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container,
                const bool& instancingEnabled = false
        ) : Entity("GraphicsObject", container, instancingEnabled) {
            add<Transform3dComponent>(transform3d());
            add<BaseMeshComponent<T>>();
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container,
                const std::string &tag,
                const Transform3dComponent& transform,
                const bool& instancingEnabled = false
        ) : Entity(tag, container, instancingEnabled) {
            add<Transform3dComponent>(transform);
            add<BaseMeshComponent<T>>();
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container,
                const std::string &tag,
                const BaseMeshComponent<T> &mesh,
                const bool& instancingEnabled = false
        ) : Entity(tag, container, instancingEnabled) {
            add<Transform3dComponent>(transform3d());
            add<BaseMeshComponent<T>>(mesh);
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container,
                const std::string &tag,
                const Transform3dComponent &transform,
                const BaseMeshComponent<T>& mesh,
                const bool& instancingEnabled = false
        ) : Entity(tag, container, instancingEnabled) {
            add<Transform3dComponent>(transform);
            add<BaseMeshComponent<T>>(mesh);
            add<MaterialComponent>();
        }

        ~Object3d() = default;
    };

    template<typename T>
    void updateObjects3d(entt::registry& registry) {
        auto objects = registry.group<Transform3dComponent, BaseMeshComponent<T>>();
        for (auto [entity, transform, mesh] : objects.each()) {
            transform.isUpdated = true;
            mesh.isUpdated = true;
        }
    }

    template<typename T>
    void updateObjects3d(const Ref<Scene> &scene) {
        updateObjects3d<T>(scene->getBatchRegistry());
        updateObjects3d<T>(scene->getInstanceRegistry());
    }
}
