//
// Created by mecha on 26.09.2021.
//

#include <math/ViewMatrices.h>

namespace engine::math {

    void update(ViewMatrix2d &viewMatrix2D) {
        viewMatrix2D.isUpdated = true;
        auto identity = glm::mat4(1);
        auto rotMat = glm::rotate(identity, viewMatrix2D.rotation, {0, 0, 1});
        viewMatrix2D.value = glm::translate(rotMat, -viewMatrix2D.position);
    }

    void update(ViewMatrix3d &viewMatrix3D) {
        viewMatrix3D.isUpdated = true;
        viewMatrix3D.position.isUpdated = true;
        auto yAxis = glm::vec3(0, 1, 0);
        auto zAxis = glm::vec3(0, 0, 1);
        const auto& pos = viewMatrix3D.position.value;
        viewMatrix3D.value = glm::lookAt(pos, pos  + zAxis, yAxis);
    }

    void serialize(YAML::Emitter& out, const char* key, const ViewMatrix2d& vm) {
        out << YAML::BeginMap;
        out << YAML::Key << key;
        yaml::serialize(out, "name", vm.name);
        yaml::serialize(out, "position", vm.position);
        yaml::serialize(out, "rotation", vm.rotation);
        out << YAML::EndMap;
    }

    void deserialize(const YAML::Node& parent, const char* key, ViewMatrix2d& vm) {
        auto root = parent[key];
        if (root) {
            yaml::deserialize(root, "name", vm.name);
            yaml::deserialize(root, "position", vm.position);
            yaml::deserialize(root, "rotation", vm.rotation);
        }
    }

    void serialize(YAML::Emitter& out, const char* key, const ViewMatrix3d& vm) {
        out << YAML::BeginMap;
        out << YAML::Key << key;
        yaml::serialize(out, "name", vm.name);
        yaml::serialize(out, "position", vm.position);
        yaml::serialize(out, "rotation", vm.rotation);
        yaml::serialize(out, "scale", vm.scale);
        out << YAML::EndMap;
    }

    void deserialize(const YAML::Node& parent, const char* key, ViewMatrix3d& vm) {
        auto root = parent[key];
        if (root) {
            yaml::deserialize(root, "name", vm.name);
            yaml::deserialize(root, "position", vm.position);
            yaml::deserialize(root, "rotation", vm.rotation);
            yaml::deserialize(root, "scale", vm.scale);
        }
    }
}