//
// Created by mecha on 23.09.2021.
//

#include <math/ModelMatrices.h>

namespace engine::math {

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

    void write(std::fstream& file, const ModelMatrix2d& value) {
        ::write(file, value.position);
        ::write(file, value.rotation);
        ::write(file, value.scale);
    }

    void read(std::fstream& file, ModelMatrix2d& value) {
        ::read(file, value.position);
        ::read(file, value.rotation);
        ::read(file, value.scale);
    }

    void write(std::fstream& file, const ModelMatrix3d& value) {
        ::write(file, value.position);
        ::write(file, value.rotation);
        ::write(file, value.scale);
    }

    void read(std::fstream& file, ModelMatrix3d& value) {
        ::read(file, value.position);
        ::read(file, value.rotation);
        ::read(file, value.scale);
    }

    void ModelMatrix2d::apply() {
        auto identity = math::mat4f();
        auto translationMatrix = math::translate(identity, math::vec3f(position, 1));
        auto rotationMatrix = math::rotateZ(translationMatrix, rotation);
    }

    void ModelMatrix3d::apply() {
        auto identity = glm::mat4(1);
        glm::vec3 pos = {
                position.x(),
                position.y(),
                position.z()
        };
        auto translation = glm::translate(identity, pos);
        glm::vec3 rot = {
                rotation.x(),
                rotation.y(),
                rotation.z()
        };
//        auto rotMatX = math::rotateX(identity, rot.x());
//        auto rotMatY = math::rotateY(identity, rot.y());
//        auto rotMatZ = math::rotateZ(identity, rot.z());
        auto rotMatX = glm::rotate(identity, rot.x, { 1, 0, 0 });
        auto rotMatY = glm::rotate(identity, rot.y, { 0, 1, 0 });
        auto rotMatZ = glm::rotate(identity, rot.z, { 0, 0, 1 });
        auto rotMat = rotMatZ * rotMatY * rotMatX;

        glm::vec3 s = {
                scale.x(),
                scale.y(),
                scale.z()
        };
        auto scaleMat = glm::scale(identity, s);

        auto model = translation * rotMat * scaleMat;

        value.v0 = { model[0][0], model[0][1], model[0][2], model[0][3] };
        value.v1 = { model[1][0], model[1][1], model[1][2], model[1][3] };
        value.v2 = { model[2][0], model[2][1], model[2][2], model[2][3] };
        value.v3 = { model[3][0], model[3][1], model[3][2], model[3][3] };
    }
}