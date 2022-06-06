//
// Created by mecha on 21.05.2022.
//

#include <graphics/transform/TransformComponents.h>

namespace engine::graphics {

    void Transform2dComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "Transform2dComponent";
        math::serialize(out, "modelMatrix", modelMatrix);
        out << YAML::EndMap;
    }

    void Transform2dComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["Transform2dComponent"];
        if (root) {
            math::deserialize(root, "modelMatrix", modelMatrix);
        }
    }

    void Transform3dComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "Transform3dComponent";
        math::serialize(out, "modelMatrix", modelMatrix);
        out << YAML::EndMap;
    }

    void Transform3dComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["Transform3dComponent"];
        if (root) {
            math::deserialize(root, "modelMatrix", modelMatrix);
        }
    }
}