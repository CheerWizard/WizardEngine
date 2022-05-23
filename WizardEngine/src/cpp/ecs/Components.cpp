//
// Created by mecha on 21.05.2022.
//

#include <ecs/Components.h>

namespace engine::ecs {

    void TagComponent::serialize(YAML::Emitter &out) {
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap; // TagComponent
        out << YAML::Key << "Tag" << YAML::Value << tag;
        yaml::serialize(out, "Tag", tag);
        out << YAML::EndMap; // TagComponent
    }

    void TagComponent::deserialize(const YAML::Node &parent) {
        auto tagComponent = parent["TagComponent"];
        if (tagComponent) {
            tag = tagComponent["Tag"].as<std::string>();
        }
        ENGINE_TRACE("Deserialized TAG = {0}", tag);
    }

}