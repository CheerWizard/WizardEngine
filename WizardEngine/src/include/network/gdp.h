//
// Created by mecha on 25.05.2022.
//

#pragma once

#include <core/primitives.h>
#include <serialization/serialization.h>

/**
 * Game Data Protocol - used for transferring entity and component data through TCP/UDP network
 * address - used to recognize a source : Client-to-Client, Client-to-Server, Server-to-Client
 * type - used to recognize type of data to deserialize data : ex. type: , data: "Open_Door";
 * data - actual data that should be deserialized by provided Service class from the Game.
 * */
namespace engine::network {

    using namespace core;

    struct NetworkData {
        char* data = nullptr;
        size_t size = 0;

        NetworkData(char* data, size_t size) : data(data), size(size) {}
    };

    constexpr u32 CLIENT_TO_CLIENT = 0;
    constexpr u32 CLIENT_TO_SERVER = 1;
    constexpr u32 SERVER_TO_CLIENT = 2;

    struct GDHeader : io::Serializable {
        u32 address;
        u32 type;

        GDHeader() = default;
        GDHeader(u8 address, u32 type) : address(address), type(type) {}

        void serialize(YAML::Emitter &out) override;
        void deserialize(const YAML::Node &parent) override;
    };

    typedef io::Serializable GDBody;

    class GDSerializer final {
    public:
        static NetworkData serialize(GDHeader& header, GDBody& body);
        static bool deserialize(char* gameData, std::pair<YAML::Node, GDHeader>& gdNodeHeader);
    };

    template<class V>
    struct GDPrimitive : io::Serializable {
        V value;

        GDPrimitive() = default;
        GDPrimitive(const V& value) : value(value) {}

        void serialize(YAML::Emitter &out) override;
        void deserialize(const YAML::Node &parent) override;
    };

    template<class V>
    void GDPrimitive<V>::serialize(YAML::Emitter &out) {
        yaml::serialize(out, "value", value);
    }

    template<class V>
    void GDPrimitive<V>::deserialize(const YAML::Node &parent) {
        value = parent["value"].template as<V>();
    }

    template<class V>
    struct GDPrimitives : io::Serializable {
        vector<V> values;

        GDPrimitives(const std::initializer_list<V>& values) : values(values) {}

        void serialize(YAML::Emitter &out) override;
        void deserialize(const YAML::Node &parent) override;
    };

    template<class V>
    void GDPrimitives<V>::serialize(YAML::Emitter &out) {
        out << YAML::Key << "values" << YAML::Value << YAML::Flow;
        out << YAML::BeginSeq;
        for (auto value : values) {
            out << value;
        }
        out << YAML::EndSeq;
    }

    template<class V>
    void GDPrimitives<V>::deserialize(const YAML::Node &parent) {
        YAML::Node valuesNode = parent["values"];
    }

    /**
     * V - class/struct, which should have implemented Serializable interface. Otherwise it won't compile!
     * */
    template<class V>
    struct GDSerializables : io::Serializable {
        vector<V> serializables;

        GDSerializables(const std::initializer_list<V>& serializables) : serializables(serializables) {}

        void serialize(YAML::Emitter &out) override;
        void deserialize(const YAML::Node &parent) override;
    };

    template<class V>
    void GDSerializables<V>::serialize(YAML::Emitter &out) {
        out << YAML::Key << "serializables" << YAML::Value << YAML::Flow;
        out << YAML::BeginSeq;
        for (auto value : serializables) {
            value.serialize(out);
        }
        out << YAML::EndSeq;
    }

    template<class V>
    void GDSerializables<V>::deserialize(const YAML::Node &parent) {
        YAML::Node valuesNode = parent["serializables"];
        for (auto value : serializables) {
            value.deserialize(valuesNode);
        }
    }
}
