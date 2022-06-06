//
// Created by mecha on 06.06.2022.
//

#include <graphics/outline/Outline.h>

namespace engine::graphics {

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

}