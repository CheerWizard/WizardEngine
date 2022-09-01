//
// Created by mecha on 23.09.2021.
//

#include <math/ModelMatrices.h>

namespace engine::math {

    void updateModel2d(ModelMatrix2d &modelMatrix2D) {
        auto identity = math::mat4f();
        auto translationMatrix = math::translate(identity, math::vec3f(modelMatrix2D.position, 1));
        auto rotationMatrix = math::rotateZ(translationMatrix, modelMatrix2D.rotation);
    }

    void updateModel3d(ModelMatrix3d &modelMatrix3D) {
        auto identity = glm::mat4(1);
        glm::vec3 position = {
                modelMatrix3D.position.x(),
                modelMatrix3D.position.y(),
                modelMatrix3D.position.z()
        };
        auto translation = glm::translate(identity, position);
        glm::vec3 rot = {
                modelMatrix3D.rotation.x(),
                modelMatrix3D.rotation.y(),
                modelMatrix3D.rotation.z()
        };
//        auto rotMatX = math::rotateX(identity, rot.x());
//        auto rotMatY = math::rotateY(identity, rot.y());
//        auto rotMatZ = math::rotateZ(identity, rot.z());
        auto rotMatX = glm::rotate(identity, rot.x, { 1, 0, 0 });
        auto rotMatY = glm::rotate(identity, rot.y, { 0, 1, 0 });
        auto rotMatZ = glm::rotate(identity, rot.z, { 0, 0, 1 });
        auto rotation = rotMatZ * rotMatY * rotMatX;

        glm::vec3 s = {
                modelMatrix3D.scale.x(),
                modelMatrix3D.scale.y(),
                modelMatrix3D.scale.z()
        };
        auto scale = glm::scale(identity, s);

        auto model = translation * rotation * scale;

        modelMatrix3D.value.v0 = { model[0][0], model[0][1], model[0][2], model[0][3] };
        modelMatrix3D.value.v1 = { model[1][0], model[1][1], model[1][2], model[1][3] };
        modelMatrix3D.value.v2 = { model[2][0], model[2][1], model[2][2], model[2][3] };
        modelMatrix3D.value.v3 = { model[3][0], model[3][1], model[3][2], model[3][3] };
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

    void ModelMatrix2d::apply() {
        updateModel2d(*this);
    }

    void ModelMatrix3d::apply() {
        updateModel3d(*this);
    }
}