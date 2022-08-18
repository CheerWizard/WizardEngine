//
// Created by mecha on 08.01.2022.
//

#pragma once

#include <ecs/Entity.h>
#include <graphics/core/geometry/Mesh.h>
#include <graphics/transform/TransformComponents.h>

namespace engine::graphics {

    template<typename T>
    class Object3d : public ecs::Entity {

    public:
        Object3d() = default;

        Object3d(
                const std::string &tag,
                ecs::EntityContainer* container
        ) : Entity(tag, container) {
            add<Transform3dComponent>(Transform3dComponent());
        }

        Object3d(
                ecs::EntityContainer* container
        ) : Entity("GraphicsObject", container) {
            add<Transform3dComponent>(Transform3dComponent());
            add<BaseMeshComponent<T>>();
        }

        Object3d(
                ecs::EntityContainer* container,
                const std::string &tag,
                const Transform3dComponent& transform
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
        }

        Object3d(
                ecs::EntityContainer* container,
                const std::string &tag,
                const Transform3dComponent &transform,
                const BaseMeshComponent<T>& mesh
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
            add<BaseMeshComponent<T>>(mesh);
        }

        Object3d(
                ecs::EntityContainer* container,
                const std::string& tag,
                const Transform3dComponent &transform,
                const VertexDataComponent<InstanceVertex<T>> vertexDataComponent
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
            add<VertexDataComponent<InstanceVertex<T>>>(vertexDataComponent);
        }

        Object3d(
                ecs::EntityContainer* container,
                const std::string& tag,
                const Transform3dComponent &transform,
                const VertexDataComponent<BatchVertex<T>> vertexDataComponent
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
            add<VertexDataComponent<BatchVertex<T>>>(vertexDataComponent);
        }

        ~Object3d() = default;

    public:
        ModelMatrix3d& getTransform();
        void applyTransform();
    };

    template<typename T>
    ModelMatrix3d& Object3d<T>::getTransform() {
        return get<Transform3dComponent>()->modelMatrix;
    }

    template<typename T>
    void Object3d<T>::applyTransform() {
        math::updateModel3d(getTransform());
    }
}
