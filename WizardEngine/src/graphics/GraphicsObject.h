//
// Created by mecha on 08.01.2022.
//

#pragma once

#include "../ecs/Entity.h"
#include "core/geometry/MeshComponent.h"
#include "transform/TransformComponents.h"

namespace engine {

    class Object3d : public Entity {

    public:
        Object3d(const std::string &tag, EntityContainer* container) : Entity(tag, container) {
            add<Transform3dComponent>(transform3d());
            add<MeshComponent>();
            add<MaterialComponent>();
        }

        Object3d(EntityContainer* container) : Entity("GraphicsObject", container) {
            add<Transform3dComponent>(transform3d());
            add<MeshComponent>();
            add<MaterialComponent>();
        }

        Object3d(EntityContainer* container, const std::string &tag, const Transform3dComponent &transform) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
            add<MeshComponent>();
            add<MaterialComponent>();
        }

        Object3d(EntityContainer* container, const std::string &tag, const MeshComponent &mesh) : Entity(tag, container) {
            add<Transform3dComponent>(transform3d());
            add<MeshComponent>(mesh);
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container,
                const std::string &tag,
                const Transform3dComponent &transform,
                const MeshComponent& mesh
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
            add<MeshComponent>(mesh);
            add<MaterialComponent>();
        }

        ~Object3d() = default;

    };

    void updateObjects3d(const Ref<Scene> &scene) {
        auto objects = scene->getRegistry().group<Transform3dComponent, MeshComponent>();
        for (auto [entity, transform, mesh] : objects.each()) {
            transform.isUpdated = true;
            mesh.isUpdated = true;
        }
    }

}
