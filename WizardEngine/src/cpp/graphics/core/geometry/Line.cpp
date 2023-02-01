//
// Created by mecha on 02.06.2022.
//

#include <graphics/core/geometry/Line.h>

namespace engine::graphics {

    void LineVertex::serialize(YAML::Emitter &out) {
        out << YAML::Key << "LineVertex";
        out << YAML::BeginMap;
        yaml::serialize(out, "position", position);
        yaml::serialize(out, "color", color);
        out << YAML::EndMap;
    }

    void LineVertex::deserialize(const YAML::Node &parent) {
        auto root = parent["LineVertex"];
        if (root) {
            yaml::deserialize(root, "position", position);
            yaml::deserialize(root, "color", position);
        }
    }

    void LineVertex::write(std::fstream &file) {
        ::write(file, position);
        ::write(file, color);
    }

    void LineVertex::read(std::fstream &file) {
        ::read(file, position);
        ::read(file, color);
    }

}
