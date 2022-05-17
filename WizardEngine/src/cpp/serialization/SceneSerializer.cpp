//
// Created by mecha on 17.05.2022.
//

#include <serialization/SceneSerializer.h>
#include <yaml/yaml.h>
#include <core/filesystem.h>

#include <ecs/Components.h>
#include <graphics/transform/TransformComponents.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::io {

    static void serializeEntity(YAML::Emitter& out, const ecs::Entity& entity) {
        out << YAML::BeginMap; // Entity
        yaml::serialize(out, "Entity", entity.getId());

        if (entity.has<ecs::TagComponent>()) {
            auto& tag = entity.get<ecs::TagComponent>()->tag;
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap; // TagComponent
            yaml::serialize(out, "Tag", tag);
            out << YAML::EndMap; // TagComponent
        }

        if (entity.has<graphics::Transform2dComponent>()) {
            auto* tc = entity.get<graphics::Transform2dComponent>();
            out << YAML::Key << "Transform2dComponent";
            out << YAML::BeginMap; // Transform2dComponent
            yaml::serialize(out, "Position", tc->modelMatrix.position);
            yaml::serialize(out, "Rotation", tc->modelMatrix.rotation);
            yaml::serialize(out, "Scale", tc->modelMatrix.scale);
            out << YAML::EndMap; // Transform2dComponent
        }

        if (entity.has<graphics::Transform3dComponent>()) {
            auto* tc = entity.get<graphics::Transform3dComponent>();
            out << YAML::Key << "Transform3dComponent";
            out << YAML::BeginMap; // Transform3dComponent
            yaml::serialize(out, "Position", tc->modelMatrix.position);
            yaml::serialize(out, "Rotation", tc->modelMatrix.rotation);
            yaml::serialize(out, "Scale", tc->modelMatrix.scale);
            out << YAML::EndMap; // Transform3dComponent
        }

        if (entity.has<graphics::Camera2dComponent>()) {
            auto* cameraComponent = entity.get<graphics::Camera2dComponent>();
            out << YAML::Key << "Camera2dComponent";
            out << YAML::BeginMap; // Camera2dComponent

            auto& vp = cameraComponent->viewProjection;
            out << YAML::Key << "ViewProjection" << YAML::Value;
            out << YAML::BeginMap; // ViewProjection2d

            auto& o = vp.orthographicMatrix;
            out << YAML::Key << "OrthographicProjection" << YAML::Value;
            out << YAML::BeginMap; // OrthographicProjection
            yaml::serialize(out, "Left", o.left);
            yaml::serialize(out, "Top", o.top);
            yaml::serialize(out, "Right", o.right);
            yaml::serialize(out, "Bottom", o.bottom);
            out << YAML::EndMap; // OrthographicProjection

            auto& vm = vp.viewMatrix;
            out << YAML::Key << "ViewMatrix" << YAML::Value;
            out << YAML::BeginMap; // ViewMatrix
            yaml::serialize(out, "Position", vm.position);
            yaml::serialize(out, "Rotation", vm.rotation);
            out << YAML::EndMap; // ViewMatrix

            out << YAML::EndMap; // ViewProjection2d

            out << YAML::EndMap; // Camera2dComponent
        }

        out << YAML::EndMap; // Entity
    }

    void SceneSerializer::serializeText(const char *filepath) {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << scene->getName();
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        scene->getRegistry().eachEntity([&](ecs::entity_id entityId) {
            ecs::Entity entity = { scene.get(), entityId };
            serializeEntity(out, entity);
        });

        out << YAML::EndSeq;
        out << YAML::EndMap;

        filesystem::write(filepath, out.c_str());
    }

    void SceneSerializer::serializeBinary(const char *filepath) {
    }

    bool SceneSerializer::deserializeText(const char *filepath) {
        // load data from file
        YAML::Node data;
        try {
            data = YAML::LoadFile(filepath);
        } catch (YAML::ParserException& e) {
            return false;
        }

        if (!data["Scene"]) return false;

        auto sceneName = data["Scene"].as<std::string>();
        ENGINE_TRACE("Deserializing scene '{0}'", sceneName);

        auto entities = data["Entities"];
        if (entities) {
            for (auto entity : entities) {
                std::string name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent) {
                    name = tagComponent["Tag"].as<std::string>();
                }

                ENGINE_TRACE("Deserialized entity with name = {0}", name);

                ecs::Entity deserializedEntity = ecs::Entity(name, scene.get());

                auto transform2dComponent = entity["Transform2dComponent"];
                if (transform2dComponent) {
                    auto& model = deserializedEntity.get<graphics::Transform2dComponent>()->modelMatrix;
                    model.position = transform2dComponent["Position"].as<glm::fvec2>();
                    model.rotation = transform2dComponent["Rotation"].as<f32>();
                    model.scale = transform2dComponent["Scale"].as<glm::fvec2>();
                }

                auto transform3dComponent = entity["Transform3dComponent"];
                if (transform3dComponent) {
                    auto& model = deserializedEntity.get<graphics::Transform3dComponent>()->modelMatrix;
                    model.position = transform3dComponent["Position"].as<glm::fvec3>();
                    model.rotation = transform3dComponent["Rotation"].as<glm::fvec3>();
                    model.scale = transform3dComponent["Scale"].as<glm::fvec3>();
                }
            }
        }

        return true;
    }

    bool SceneSerializer::deserializeBinary(const char *filepath) {
        return false;
    }

}