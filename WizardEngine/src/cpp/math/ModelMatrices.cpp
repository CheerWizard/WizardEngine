//
// Created by mecha on 23.09.2021.
//

#include <math/ModelMatrices.h>

namespace engine::math {

    void updateModel2d(ModelMatrix2d &modelMatrix2D) {
        modelMatrix2D.isUpdated = true;

        auto identity = math::mat4f();
        auto translationMatrix = math::translate(identity, math::vec3f(modelMatrix2D.position, 1));
        auto rotationMatrix = math::rotate(translationMatrix, modelMatrix2D.rotation, math::vec3f(0, 0, 1));
        modelMatrix2D.value = rotationMatrix * math::vec3f(modelMatrix2D.scale, 1);
    }

    void updateModel3d(ModelMatrix3d &modelMatrix3D) {
        modelMatrix3D.isUpdated = true;

        auto identity = math::mat4f();
        auto posMat = math::translate(identity, modelMatrix3D.position);

        auto& rot = modelMatrix3D.rotation;
        auto rotMatX = math::rotate(identity, rot.x(), math::vec3f(1, 0, 0));
        auto rotMatY = math::rotate(identity, rot.y(), math::vec3f(0, 1, 0));
        auto rotMatZ = math::rotate(identity, rot.z(), math::vec3f(0, 0, 1));
        auto rotMat = rotMatZ * rotMatY * rotMatX;

        auto scaleMat = identity * modelMatrix3D.scale;

        modelMatrix3D.value = posMat * rotMat * scaleMat;
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