//
// Created by mecha on 23.09.2021.
//

#include <math/ModelMatrices.h>

namespace engine::math {

    void updateModel2d(ModelMatrix2d &modelMatrix2D) {
        modelMatrix2D.isUpdated = true;

        auto identity = math::mat4f();
        auto translationMatrix = math::translate(identity, math::vec3f(modelMatrix2D.position, 1));
        auto rotationMatrix = math::rotateZ(translationMatrix, modelMatrix2D.rotation);
    }

    void updateModel3d(ModelMatrix3d &modelMatrix3D) {
        modelMatrix3D.isUpdated = true;

        mat4f identity;
        auto translation = math::translate(identity, modelMatrix3D.position);

        const auto& rot = modelMatrix3D.rotation;
        auto rotMatX = math::rotateX(identity, rot.x());
        auto rotMatY = math::rotateY(identity, rot.y());
        auto rotMatZ = math::rotateZ(identity, rot.z());
        auto rotation = rotMatZ * rotMatY * rotMatX;

        auto scale = math::scale(identity, modelMatrix3D.scale);

        modelMatrix3D.value = translation * rotation * scale;
    }

    void serialize(YAML::Emitter& out, const char* key, const ModelMatrix2d& model) {
        out << YAML::BeginMap;
        out << YAML::Key << key;
        yaml::serialize(out, "position", model.position);
        yaml::serialize(out, "rotation", model.rotation);
        yaml::serialize(out, "scale", model.scale);
        out << YAML::EndMap;
    }

    void deserialize(const YAML::Node& parent, const char* key, ModelMatrix2d& model) {
        auto root = parent[key];
        if (root) {
            yaml::deserialize(parent, "position", model.position);
            yaml::deserialize(parent, "rotation", model.rotation);
            yaml::deserialize(parent, "scale", model.scale);
        }
    }

    void serialize(YAML::Emitter& out, const char* key, const ModelMatrix3d& model) {
        out << YAML::BeginMap;
        out << YAML::Key << key;
        yaml::serialize(out, "position", model.position);
        yaml::serialize(out, "rotation", model.rotation);
        yaml::serialize(out, "scale", model.scale);
        out << YAML::EndMap;
    }

    void deserialize(const YAML::Node& parent, const char* key, ModelMatrix3d& model) {
        auto root = parent[key];
        if (root) {
            yaml::deserialize(parent, "position", model.position);
            yaml::deserialize(parent, "rotation", model.rotation);
            yaml::deserialize(parent, "scale", model.scale);
        }
    }
}