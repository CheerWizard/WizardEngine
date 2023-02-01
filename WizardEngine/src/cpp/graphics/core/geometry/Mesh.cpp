#include <graphics/core/geometry/Mesh.h>

namespace engine::graphics {

    void Vertex3d::serialize(YAML::Emitter &out) {
        out << YAML::Key << "Vertex3d";
        out << YAML::BeginMap;
        yaml::serialize(out, "position", position);
        yaml::serialize(out, "uv", uv);
        yaml::serialize(out, "normal", normal);
        yaml::serialize(out, "tangent", tangent);
        yaml::serialize(out, "bitangent", bitangent);
        out << YAML::EndMap;
    }

    void Vertex3d::deserialize(const YAML::Node &parent) {
        auto root = parent["Vertex3d"];
        if (root) {
            yaml::deserialize(root, "position", position);
            yaml::deserialize(root, "uv", uv);
            yaml::deserialize(root, "normal", normal);
            yaml::deserialize(root, "tangent", tangent);
            yaml::deserialize(root, "bitangent", bitangent);
        }
    }

    void Vertex3d::read(std::fstream &file) {
        ::read(file, position);
        ::read(file, uv);
        ::read(file, normal);
        ::read(file, tangent);
        ::read(file, bitangent);
    }

    void Vertex3d::write(std::fstream &file) {
        ::write(file, position);
        ::write(file, uv);
        ::write(file, normal);
        ::write(file, tangent);
        ::write(file, bitangent);
    }
}