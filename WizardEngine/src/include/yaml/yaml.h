//
// Created by mecha on 17.05.2022.
//

#pragma once

#include <core/primitives.h>
#include <core/uuid.h>
#include <core/array.h>
#include <graphics/core/shader/Uniform.h>

#include <yaml-cpp/yaml.h>

namespace YAML {

    using namespace engine::core;

    template<>
    struct convert<glm::vec2> {

        static Node encode(const glm::vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs) {
            if (!node.IsSequence() || node.size() != 2) return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec3> {

        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4> {

        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs) {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<engine::uuid> {

        static Node encode(const engine::uuid& uuid) {
            Node node;
            node.push_back(uuid);
            return node;
        }

        static bool decode(const Node& node, engine::uuid& uuid) {
            uuid = node[0].as<engine::uuid>();
            return true;
        }
    };

    template<typename T>
    struct convert<array<T>> {

        static Node encode(const array<T>& array) {
            Node node;
            array.forEach([&node](const T& item) {
               node.push_back(item);
            });
            return node;
        }

        static bool decode(const Node& node, array<T>& array) {
            array.offset = node["offset"].as<u32>();
            array.size = node["size"].as<u32>();
            array.forEach([&node](T& item, u32 i) {
                item = node[i].as<T>();
            });
            return true;
        }
    };
}

namespace engine::yaml {

    using namespace core;

    void serialize(YAML::Emitter& out, const char* key, const glm::vec2& v);
    void serialize(YAML::Emitter& out, const char* key, const glm::vec3& v);
    void serialize(YAML::Emitter& out, const char* key, const glm::vec4& v);

    template<typename T>
    void serialize(YAML::Emitter& out, const char* key, const T& v) {
        out << YAML::Key << key << YAML::Value << v;
    }

    template<typename T>
    void serialize(YAML::Emitter& out, const char* key, const shader::Uniform<T>& uniform) {
        out << YAML::BeginMap;
        out << YAML::Key << key;
        serialize(out, "name", uniform.name);
        serialize(out, "value", uniform.value);
        serialize(out, "isUpdated", uniform.isUpdated);
        out << YAML::EndMap;
    }

    void deserialize(const YAML::Node& parent, const char* key, const char* value);

    template<typename T>
    void deserialize(const YAML::Node& parent, const char* key, shader::Uniform<T>& uniform) {
        auto root = parent[key];
        if (root) {
            uniform.name = root["name"].as<std::string>().c_str();
            uniform.value = root["value"].as<T>();
            uniform.isUpdated = root["isUpdated"].as<bool>();
        }
    }

    template<typename T>
    void deserialize(const YAML::Node& parent, const char* key, T& value) {
        value = parent[key].as<T>();
    }

    template<typename T, typename Function>
    void serialize(YAML::Emitter& out, const char* key, const array<T>& array, const Function& function) {
        out << YAML::BeginMap;
        out << YAML::Key << key;

        serialize(out, "offset", array.offset);
        serialize(out, "size", array.size);

        out << YAML::Key << "values" << YAML::Value << YAML::Flow;
        out << YAML::BeginSeq;
        array.forEach([&out, &function](const T& item) {
            function(out, item);
        });
        out << YAML::EndSeq;

        out << YAML::EndMap;
    }

    template<typename T>
    void serialize(YAML::Emitter& out, const char* key, const array<T>& array) {
        serialize(out, key, array, [](YAML::Emitter& out, const T& item) {
            out << item;
        });
    }

    template<typename T>
    void deserialize(const YAML::Node& parent, const char* key, array<T>& array) {
        array = parent[key].as<engine::core::array<T>>();
    }
}