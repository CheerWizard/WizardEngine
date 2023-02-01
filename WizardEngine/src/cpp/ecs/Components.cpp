//
// Created by mecha on 21.05.2022.
//

#include <ecs/Components.h>

namespace engine::ecs {

    void TagComponent::serialize(YAML::Emitter &out) {
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap; // TagComponent
        yaml::serialize(out, "tag", tag.c_str());
        out << YAML::EndMap; // TagComponent
    }

    void TagComponent::deserialize(const YAML::Node &parent) {
        auto tagComponent = parent["TagComponent"];
        if (tagComponent) {
            tag = tagComponent["tag"].as<std::string>();
        }
        ENGINE_TRACE("Deserialized TAG = {0}", tag);
    }

    void TagComponent::read(std::fstream &file) {
        ::read(file, tag);
    }

    void TagComponent::write(std::fstream &file) {
        ::write(file, tag);
    }

}