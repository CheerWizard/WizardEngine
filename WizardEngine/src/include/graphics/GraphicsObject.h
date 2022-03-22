//
// Created by mecha on 08.01.2022.
//

#pragma once

#include <ecs/Entity.h>
#include "core/geometry/Mesh.h"
#include "transform/TransformComponents.h"

namespace engine {

    template<typename T>
    class Object3d : public Entity {

    public:
        Object3d(
                const std::string &tag,
                EntityContainer* container
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform3d());
            add<BaseMeshComponent<T>>();
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container
        ) : Entity("GraphicsObject", container) {
            add<Transform3dComponent>(transform3d());
            add<BaseMeshComponent<T>>();
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container,
                const std::string &tag,
                const Transform3dComponent& transform
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
            add<BaseMeshComponent<T>>();
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container,
                const std::string &tag,
                const BaseMeshComponent<T> &mesh
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform3d());
            add<BaseMeshComponent<T>>(mesh);
            add<MaterialComponent>();
        }

        Object3d(
                EntityContainer* container,
                const std::string &tag,
                const Transform3dComponent &transform,
                const BaseMeshComponent<T>& mesh
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
            add<BaseMeshComponent<T>>(mesh);
            add<MaterialComponent>();
        }

        Object3d(
            EntityContainer* container,
            const std::string& tag,
            const Transform3dComponent &transform,
            const VertexDataComponent<InstanceVertex<T>> vertexDataComponent
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
            add<VertexDataComponent<InstanceVertex<T>>>(vertexDataComponent);
        }

        Object3d(
                EntityContainer* container,
                const std::string& tag,
                const Transform3dComponent &transform,
                const VertexDataComponent<BatchVertex<T>> vertexDataComponent
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
            add<VertexDataComponent<BatchVertex<T>>>(vertexDataComponent);
        }

        ~Object3d() = default;
    };
}
