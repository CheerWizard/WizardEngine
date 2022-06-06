//
// Created by mecha on 21.05.2022.
//

#include <graphics/camera/CameraComponents.h>

namespace engine::graphics {

    void Camera2dComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "Camera2dComponent";
        math::serialize(out, "viewProjection", viewProjection);
        out << YAML::EndMap;
    }

    void Camera2dComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["Camera2dComponent"];
        if (root) {
            math::deserialize(root, "viewProjection", viewProjection);
        }
    }

    void Camera3dComponent::serialize(YAML::Emitter &out) {
        out << YAML::Key << "Camera3dComponent";
        out << YAML::BeginMap;
        math::serialize(out, "viewProjection", viewProjection);
        out << YAML::EndMap;
    }

    void Camera3dComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["Camera3dComponent"];
        if (root) {
            math::deserialize(root, "viewProjection", viewProjection);
        }
    }
}