//
// Created by mecha on 17.05.2022.
//

#include <yaml/yaml.h>

namespace engine::yaml {

    void serialize(YAML::Emitter& out, const char* key, const glm::vec2& v) {
        out << YAML::Key << key << YAML::Value;
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    }

    void serialize(YAML::Emitter& out, const char* key, const glm::vec3& v) {
        out << YAML::Key << key << YAML::Value;
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    }

    void serialize(YAML::Emitter& out, const char* key, const glm::vec4& v) {
        out << YAML::Key << key << YAML::Value;
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
    }

    void deserialize(const YAML::Node& parent, const char* key, const char* value) {
        value = parent[key].as<std::string>().c_str();
    }

}