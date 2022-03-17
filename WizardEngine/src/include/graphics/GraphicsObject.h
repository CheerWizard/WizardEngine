//
// Created by mecha on 08.01.2022.
//

#pragma once

#include <ecs/Entity.h>
#include "core/geometry/MeshComponent.h"
#include "transform/TransformComponents.h"

namespace engine {

    class Object3d : public Entity {

    public:
        Object3d(
                const std::string &tag,
                EntityContainer* container,
                const bool& instancingEnabled = false
        ) : Entity(tag, container, instancingEnabled) {
            add<Transform3dComponent>(transform3d());
            add<MeshComponent>();
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container,
                const bool& instancingEnabled = false
        ) : Entity("GraphicsObject", container, instancingEnabled) {
            add<Transform3dComponent>(transform3d());
            add<MeshComponent>();
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container,
                const std::string &tag,
                const Transform3dComponent& transform,
                const bool& instancingEnabled = false
        ) : Entity(tag, container, instancingEnabled) {
            add<Transform3dComponent>(transform);
            add<MeshComponent>();
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container,
                const std::string &tag,
                const MeshComponent &mesh,
                const bool& instancingEnabled = false
        ) : Entity(tag, container, instancingEnabled) {
            add<Transform3dComponent>(transform3d());
            add<MeshComponent>(mesh);
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container,
                const std::string &tag,
                const Transform3dComponent &transform,
                const MeshComponent& mesh,
                const bool& instancingEnabled = false
        ) : Entity(tag, container, instancingEnabled) {
            add<Transform3dComponent>(transform);
            add<MeshComponent>(mesh);
            add<MaterialComponent>();
        }

        ~Object3d() = default;
    };

    void updateObjects3d(entt::registry& registry) {
        auto objects = registry.group<Transform3dComponent, MeshComponent>();
        for (auto [entity, transform, mesh] : objects.each()) {
            transform.isUpdated = true;
            mesh.isUpdated = true;
        }
    }

    void updateObjects3d(const Ref<Scene> &scene) {
        updateObjects3d(scene->getBatchRegistry());
        updateObjects3d(scene->getInstanceRegistry());
    }
}
