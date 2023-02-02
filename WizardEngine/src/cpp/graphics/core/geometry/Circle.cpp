#include <graphics/core/geometry/Circle.h>

namespace engine::graphics {

    void CircleVertex::serialize(YAML::Emitter &out) {
        out << YAML::Key << "CircleVertex";
        out << YAML::BeginMap;
        yaml::serialize(out, "position", position);
        yaml::serialize(out, "uv", uv);
        out << YAML::EndMap;
    }

    void CircleVertex::deserialize(const YAML::Node &parent) {
        auto root = parent["CircleVertex"];
        if (root) {
            yaml::deserialize(root, "position", position);
            yaml::deserialize(root, "uv", uv);
        }
    }

    void CircleVertex::read(std::fstream &file) {
        ::read(file, position);
        ::read(file, uv);
    }

    void CircleVertex::write(std::fstream &file) {
        ::write(file, position);
        ::write(file, uv);
    }

    void CircleComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "CircleComponent";
        yaml::serialize(out, "name", name);
        yaml::serialize(out, "color", color);
        yaml::serialize(out, "thickness", thickness);
        yaml::serialize(out, "fade", fade);
        out << YAML::EndMap;
    }

    void CircleComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["CircleComponent"];
        if (root) {
            yaml::deserialize(root, "name", name);
            yaml::deserialize(root, "color", color);
            yaml::deserialize(root, "thickness", thickness);
            yaml::deserialize(root, "fade", fade);
        }
    }

    void CircleComponent::read(std::fstream &file) {
        ::read(file, color);
        ::read(file, thickness);
        ::read(file, fade);
    }

    void CircleComponent::write(std::fstream &file) {
        ::write(file, color);
        ::write(file, thickness);
        ::write(file, fade);
    }

}