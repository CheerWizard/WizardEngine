//
// Created by mecha on 02.06.2022.
//

#include <graphics/core/geometry/Line.h>

namespace engine::graphics {

    void BatchLineSerializable::serialize(YAML::Emitter &out) {
        out << YAML::Key << "BatchLine";
        out << YAML::BeginMap;
        yaml::serialize(out, "vertexData", line.vertexData, [](YAML::Emitter& out, const BatchVertex<LineVertex>& vertex) {
            yaml::serialize(out, "position", vertex.vertex.position);
            yaml::serialize(out, "color", vertex.vertex.color);
            yaml::serialize(out, "id", vertex.id);
        });
        yaml::serialize(out, "renderModelId", line.renderModelId);
        yaml::serialize(out, "drawType", (u8) line.drawType);
        out << YAML::EndMap;
    }

    void BatchLineSerializable::deserialize(const YAML::Node &parent) {
        auto root = parent["BatchLine"];
        if (root) {
            yaml::deserialize(root, "vertexData", line.vertexData);
            yaml::deserialize(root, "renderModelId", line.renderModelId);
            yaml::deserialize(root, "drawType", line.drawType);
        }
    }

    void InstanceLineSerializable::serialize(YAML::Emitter &out) {
        out << YAML::Key << "InstanceLine";
        out << YAML::BeginMap;
        yaml::serialize(out, "vertexData", line.vertexData, [](YAML::Emitter& out, const InstanceVertex<LineVertex>& vertex) {
            yaml::serialize(out, "position", vertex.vertex.position);
            yaml::serialize(out, "color", vertex.vertex.color);
        });
        yaml::serialize(out, "renderModelId", line.renderModelId);
        yaml::serialize(out, "drawType", (u8) line.drawType);
        out << YAML::EndMap;
    }

    void InstanceLineSerializable::deserialize(const YAML::Node &parent) {
        auto root = parent["InstanceLine"];
        if (root) {
            yaml::deserialize(root, "vertexData", line.vertexData);
            yaml::deserialize(root, "renderModelId", line.renderModelId);
            yaml::deserialize(root, "drawType", line.drawType);
        }
    }

}
