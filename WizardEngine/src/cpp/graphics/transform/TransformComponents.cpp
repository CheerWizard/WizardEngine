//
// Created by mecha on 21.05.2022.
//

#include <graphics/transform/TransformComponents.h>

namespace engine::graphics {

    void Transform2dComponent::serialize(YAML::Emitter &out) {
        out << YAML::Key << "Transform2dComponent";
        out << YAML::BeginMap; // Transform2dComponent
        yaml::serialize(out, "Position", modelMatrix.position);
        yaml::serialize(out, "Rotation", modelMatrix.rotation);
        yaml::serialize(out, "Scale", modelMatrix.scale);
        out << YAML::EndMap; // Transform2dComponent
    }

    void Transform2dComponent::deserialize(const YAML::Node &parent) {
        auto transform2dComponent = parent["Transform2dComponent"];
        if (transform2dComponent) {
            modelMatrix.position = transform2dComponent["Position"].as<glm::fvec2>();
            modelMatrix.rotation = transform2dComponent["Rotation"].as<f32>();
            modelMatrix.scale = transform2dComponent["Scale"].as<glm::fvec2>();
        }
    }

    void Transform3dComponent::serialize(YAML::Emitter &out) {
        out << YAML::Key << "Transform3dComponent";
        out << YAML::BeginMap; // Transform3dComponent
        yaml::serialize(out, "Position", modelMatrix.position);
        yaml::serialize(out, "Rotation", modelMatrix.rotation);
        yaml::serialize(out, "Scale", modelMatrix.scale);
        out << YAML::EndMap; // Transform3dComponent
    }

    void Transform3dComponent::deserialize(const YAML::Node &parent) {
        auto transform3dComponent = parent["Transform3dComponent"];
        if (transform3dComponent) {
            modelMatrix.position = transform3dComponent["Position"].as<glm::fvec3>();
            modelMatrix.rotation = transform3dComponent["Rotation"].as<glm::fvec3>();
            modelMatrix.scale = transform3dComponent["Scale"].as<glm::fvec3>();
        }
    }
}