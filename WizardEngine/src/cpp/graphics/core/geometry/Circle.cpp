//
// Created by mecha on 06.06.2022.
//

#include <graphics/core/geometry/Circle.h>

namespace engine::graphics {

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

}