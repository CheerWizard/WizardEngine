//
// Created by mecha on 17.05.2022.
//

#include <yaml/yaml.h>

namespace engine::yaml {

    void deserialize(const YAML::Node& parent, const char* key, const char* value) {
        value = parent[key].as<std::string>().c_str();
    }

}