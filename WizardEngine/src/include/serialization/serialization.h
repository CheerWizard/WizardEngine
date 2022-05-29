//
// Created by mecha on 21.05.2022.
//

#pragma once

#include <sstream>
#include <yaml/yaml.h>

namespace engine::io {

        class Serializable {

        public:
            Serializable() = default;
            virtual ~Serializable() = default;

        public:
            virtual void serialize(YAML::Emitter& out) = 0;
            virtual void deserialize(const YAML::Node& parent) = 0;

        public:
            template<class T>
            static T deserialize(const YAML::Node& parent) {
                T t = T();
                t.deserialize(parent);
                return t;
            }
        };

#define deserialize_from(type, parent) engine::io::Serializable::deserialize<type>(parent)

}