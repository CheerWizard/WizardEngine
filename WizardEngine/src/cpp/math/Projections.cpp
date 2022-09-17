//
// Created by mecha on 25.09.2021.
//

#include <math/Projections.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace engine::math {

    void serialize(YAML::Emitter& out, const char* key, const OrthographicMatrix& projection) {
        out << YAML::BeginMap;
        out << YAML::Key << key;
        yaml::serialize(out, "name", projection.name);
        yaml::serialize(out, "left", projection.left);
        yaml::serialize(out, "right", projection.right);
        yaml::serialize(out, "bottom", projection.bottom);
        yaml::serialize(out, "top", projection.top);
        out << YAML::EndMap;
    }

    void deserialize(const YAML::Node& parent, const char* key, OrthographicMatrix& projection) {
        auto root = parent[key];
        if (root) {
            yaml::deserialize(root, "name", projection.name);
            yaml::deserialize(root, "left", projection.left);
            yaml::deserialize(root, "right", projection.right);
            yaml::deserialize(root, "bottom", projection.bottom);
            yaml::deserialize(root, "top", projection.top);
        }
    }

    void serialize(YAML::Emitter& out, const char* key, const PerspectiveMatrix& projection) {
        out << YAML::BeginMap;
        out << YAML::Key << key;
        yaml::serialize(out, "name", projection.name);
        yaml::serialize(out, "aspectRatio", projection.aspectRatio);
        yaml::serialize(out, "fov", projection.fieldOfView);
        yaml::serialize(out, "zNear", projection.zNear);
        yaml::serialize(out, "zFar", projection.zFar);
        out << YAML::EndMap;
    }

    void deserialize(const YAML::Node& parent, const char* key, PerspectiveMatrix& projection) {
        auto root = parent[key];
        if (root) {
            yaml::deserialize(root, "name", projection.name);
            yaml::deserialize(root, "aspectRatio", projection.aspectRatio);
            yaml::deserialize(root, "fov", projection.fieldOfView);
            yaml::deserialize(root, "zNear", projection.zNear);
            yaml::deserialize(root, "zFar", projection.zFar);
        }
    }

    void OrthographicMatrix::apply() {
        value = math::ortho(left, right, bottom, top, -1.0f, 1.0f);
    }

    void PerspectiveMatrix::apply() {
        value = math::perspective(fieldOfView, aspectRatio, zNear, zFar);
    }
}