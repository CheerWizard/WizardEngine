//
// Created by mecha on 21.05.2022.
//

#include <graphics/transform/TransformComponents.h>

namespace engine::graphics {

    void Transform2dComponent::serialize(YAML::Emitter &out) {
        out << YAML::Key << "Transform2dComponent";
        out << YAML::BeginMap; // Transform2dComponent
        yaml::serialize(out, "position", modelMatrix.position);
        yaml::serialize(out, "rotation", modelMatrix.rotation);
        yaml::serialize(out, "scale", modelMatrix.scale);
        out << YAML::EndMap; // Transform2dComponent
    }

    void Transform2dComponent::deserialize(const YAML::Node &parent) {
        auto transform2dComponent = parent["Transform2dComponent"];
        if (transform2dComponent) {
            modelMatrix.position = transform2dComponent["position"].as<glm::fvec2>();
            modelMatrix.rotation = transform2dComponent["rotation"].as<f32>();
            modelMatrix.scale = transform2dComponent["scale"].as<glm::fvec2>();
        }
    }

    void Transform3dComponent::serialize(YAML::Emitter &out) {
        out << YAML::Key << "Transform3dComponent";
        out << YAML::BeginMap; // Transform3dComponent
        yaml::serialize(out, "position", modelMatrix.position);
        yaml::serialize(out, "rotation", modelMatrix.rotation);
        yaml::serialize(out, "scale", modelMatrix.scale);
        out << YAML::EndMap; // Transform3dComponent
    }

    void Transform3dComponent::deserialize(const YAML::Node &parent) {
        auto transform3dComponent = parent["Transform3dComponent"];
        if (transform3dComponent) {
            modelMatrix.position = transform3dComponent["position"].as<glm::fvec3>();
            modelMatrix.rotation = transform3dComponent["rotation"].as<glm::fvec3>();
            modelMatrix.scale = transform3dComponent["scale"].as<glm::fvec3>();
        }
    }
}