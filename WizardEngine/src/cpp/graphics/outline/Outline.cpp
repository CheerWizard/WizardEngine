#include <graphics/outline/Outline.h>

namespace engine::graphics {

    void OutlineVertex::serialize(YAML::Emitter &out) {
        out << YAML::Key << "OutlineVertex";
        out << YAML::BeginMap;
        yaml::serialize(out, "position", position);
        yaml::serialize(out, "normal", normal);
        out << YAML::EndMap;
    }

    void OutlineVertex::deserialize(const YAML::Node& parent) {
        auto root = parent["OutlineVertex"];
        if (root) {
            yaml::deserialize(root, "position", position);
            yaml::deserialize(root, "normal", normal);
        }
    }

    void OutlineVertex::read(std::fstream &file) {
        ::read(file, position);
        ::read(file, normal);
    }

    void OutlineVertex::write(std::fstream &file) {
        ::write(file, position);
        ::write(file, normal);
    }

    void OutlineComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "OutlineComponent";
        yaml::serialize(out, "name", name);
        yaml::serialize(out, "color", color);
        yaml::serialize(out, "thickness", thickness);
        out << YAML::EndMap;
    }

    void OutlineComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["OutlineComponent"];
        if (root) {
            yaml::deserialize(parent, "name", name);
            yaml::deserialize(parent, "color", color);
            yaml::deserialize(parent, "thickness", thickness);
        }
    }

    void OutlineComponent::read(std::fstream &file) {
        ::read(file, color);
        ::read(file, thickness);
    }

    void OutlineComponent::write(std::fstream &file) {
        ::write(file, color);
        ::write(file, thickness);
    }
}