//
// Created by mecha on 26.09.2021.
//

#include <math/ViewMatrices.h>
#include <glm/gtx/quaternion.hpp>

namespace engine::math {

    void update(ViewMatrix2d &viewMatrix2D) {
        math::mat4f identity;
        auto rotMat = math::rotateZ(identity, viewMatrix2D.rotation);
        viewMatrix2D.value = math::translate(rotMat, -viewMatrix2D.position);
    }

    void update(ViewMatrix3d &viewMatrix3D) {
//        vec3f yAxis = { 0, 1, 0 };
//        vec3f zAxis = { 0, 0, 1 };
//        viewMatrix3D.value = math::lookAt(pos, pos + zAxis, yAxis);

        const auto& pos = viewMatrix3D.position.value;
        glm::vec3 position = { pos.x(), pos.y(), pos.z() };
        auto v = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(viewMatrix3D.orientation());
        v = glm::inverse(v);

        viewMatrix3D.value.v0 = { v[0][0], v[0][1], v[0][2], v[0][3] };
        viewMatrix3D.value.v1 = { v[1][0], v[1][1], v[1][2], v[1][3] };
        viewMatrix3D.value.v2 = { v[2][0], v[2][1], v[2][2], v[2][3] };
        viewMatrix3D.value.v3 = { v[3][0], v[3][1], v[3][2], v[3][3] };
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
        yaml::serialize(out, "pitch", vm.pitch);
        yaml::serialize(out, "yaw", vm.yaw);
        yaml::serialize(out, "scale", vm.scale);
        out << YAML::EndMap;
    }

    void deserialize(const YAML::Node& parent, const char* key, ViewMatrix3d& vm) {
        auto root = parent[key];
        if (root) {
            yaml::deserialize(root, "name", vm.name);
            yaml::deserialize(root, "position", vm.position);
            yaml::deserialize(root, "pitch", vm.pitch);
            yaml::deserialize(root, "yaw", vm.yaw);
            yaml::deserialize(root, "scale", vm.scale);
        }
    }

    glm::quat ViewMatrix3d::orientation() const {
        return { glm::vec3(-pitch, -yaw, 0) };
    }

    vec3f ViewMatrix3d::forwardDirection() const {
        auto v = glm::rotate(orientation(), glm::vec3(0.0f, 0.0f, -1.0f));
        return { v.x, v.y, v.z };
    }

    vec3f ViewMatrix3d::rightDirection() const {
        auto v = glm::rotate(orientation(), glm::vec3(1.0f, 0.0f, 0.0f));
        return { v.x, v.y, v.z };
    }

    vec3f ViewMatrix3d::upDirection() const {
        auto v = glm::rotate(orientation(), glm::vec3(0.0f, 1.0f, 0.0f));
        return { v.x, v.y, v.z };
    }
}