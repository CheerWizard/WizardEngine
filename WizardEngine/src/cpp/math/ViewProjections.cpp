//
// Created by mecha on 23.09.2021.
//

#include <math/ViewProjections.h>

namespace engine::math {

    void ViewProjections::update(ViewProjection2d &viewProjection2D) {
        viewProjection2D.isUpdated = true;
        math::update(viewProjection2D.orthographicMatrix);
        math::update(viewProjection2D.viewMatrix);
        viewProjection2D.value = viewProjection2D.orthographicMatrix.value * viewProjection2D.viewMatrix.value;
    }

    void ViewProjections::update(ViewProjection3d &viewProjection3D) {
        viewProjection3D.isUpdated = true;
        math::update(viewProjection3D.perspectiveMatrix);
        math::update(viewProjection3D.viewMatrix);
        viewProjection3D.value = viewProjection3D.viewMatrix.value * viewProjection3D.perspectiveMatrix.value;
    }

    void serialize(YAML::Emitter& out, const char* key, const ViewProjection2d& vp) {
        out << YAML::BeginMap;
        out << YAML::Key << key;
        yaml::serialize(out, "name", vp.name);
        serialize(out, "viewMatrix", vp.viewMatrix);
        serialize(out, "orthographicMatrix", vp.orthographicMatrix);
        out << YAML::EndMap;
    }

    void deserialize(const YAML::Node& parent, const char* key, ViewProjection2d& vp) {
        auto root = parent[key];
        if (root) {
            yaml::deserialize(root, "name", vp.name);
            deserialize(root, "viewMatrix", vp.viewMatrix);
            deserialize(root, "orthographicMatrix", vp.orthographicMatrix);
        }
    }

    void serialize(YAML::Emitter& out, const char* key, const ViewProjection3d& vp) {
        out << YAML::BeginMap;
        out << YAML::Key << key;
        yaml::serialize(out, "name", vp.name);
        serialize(out, "viewMatrix", vp.viewMatrix);
        serialize(out, "perspectiveMatrix", vp.perspectiveMatrix);
        out << YAML::EndMap;
    }

    void deserialize(const YAML::Node& parent, const char* key, ViewProjection3d& vp) {
        auto root = parent[key];
        if (root) {
            yaml::deserialize(root, "name", vp.name);
            deserialize(root, "viewMatrix", vp.viewMatrix);
            deserialize(root, "perspectiveMatrix", vp.perspectiveMatrix);
        }
    }

}