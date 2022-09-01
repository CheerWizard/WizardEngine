//
// Created by mecha on 08.01.2022.
//

#pragma once

#include <ecs/Entity.h>
#include <graphics/core/geometry/Mesh.h>
#include <graphics/transform/TransformComponents.h>

namespace engine::graphics {

    template<typename T>
    class Object : public ecs::Entity {

    public:
        Object() = default;

        Object(
                const std::string &tag,
                ecs::EntityContainer* container
        ) : Entity(tag, container) {
            add<Transform3dComponent>(Transform3dComponent());
        }

        Object(
                ecs::EntityContainer* container
        ) : Entity("GraphicsObject", container) {
            add<Transform3dComponent>(Transform3dComponent());
            add<BaseMeshComponent<T>>();
        }

        Object(
                ecs::EntityContainer* container,
                const std::string &tag,
                const Transform3dComponent& transform
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
        }

        Object(
                ecs::EntityContainer* container,
                const std::string &tag,
                const Transform3dComponent &transform,
                const BaseMeshComponent<T>& mesh
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
            add<BaseMeshComponent<T>>(mesh);
        }

        Object(
                ecs::EntityContainer* container,
                const std::string& tag,
                const Transform3dComponent &transform,
                const VertexDataComponent<InstanceVertex<T>> vertexDataComponent
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
            add<VertexDataComponent<InstanceVertex<T>>>(vertexDataComponent);
        }

        Object(
                ecs::EntityContainer* container,
                const std::string& tag,
                const Transform3dComponent &transform,
                const VertexDataComponent<BatchVertex<T>> vertexDataComponent
        ) : Entity(tag, container) {
            add<Transform3dComponent>(transform);
            add<VertexDataComponent<BatchVertex<T>>>(vertexDataComponent);
        }

        ~Object() = default;

    public:
        ModelMatrix3d& getTransform();
        void applyTransform();
        BaseMeshComponent<BatchVertex<Vertex3d>>& getBatch3d();
    };

    template<typename T>
    ModelMatrix3d& Object<T>::getTransform() {
        return get<Transform3dComponent>()->modelMatrix;
    }

    template<typename T>
    void Object<T>::applyTransform() {
        getTransform().apply();
    }

    template<typename T>
    BaseMeshComponent<BatchVertex<Vertex3d>>& Object<T>::getBatch3d() {
        return *get<BaseMeshComponent<BatchVertex<Vertex3d>>>();
    }

    typedef Object<BatchVertex<Vertex3d>> Batch3d;
    typedef Object<InstanceVertex<Vertex3d>> Instance3d;
}
