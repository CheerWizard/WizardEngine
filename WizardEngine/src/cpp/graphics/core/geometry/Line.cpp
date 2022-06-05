//
// Created by mecha on 02.06.2022.
//

#include <graphics/core/geometry/Line.h>

namespace engine::graphics {

    void BatchLineSerializable::serialize(YAML::Emitter &out) {
        out << YAML::Key << "BatchLine";
        out << YAML::BeginMap;

        out << YAML::Key << "VertexData";
        out << YAML::BeginMap;
        yaml::serialize(out, "start", line.vertexData.vertexStart);
        yaml::serialize(out, "count", line.vertexData.vertexCount);
        out << YAML::EndMap;

        yaml::serialize(out, "renderModelId", line.renderModelId);
        yaml::serialize(out, "drawType", (u8) line.drawType);

        out << YAML::EndMap;
    }

    void BatchLineSerializable::deserialize(const YAML::Node &parent) {
        auto lineNode = parent["BatchLine"];
        if (lineNode) {
            auto vertexDataNode = lineNode["VertexData"];
            line.vertexData.vertexStart = lineNode["start"].as<u32>();
            line.vertexData.vertexCount = lineNode["count"].as<u32>();
            line.renderModelId = lineNode["renderModelId"].as<u32>();
            line.drawType = static_cast<DrawType>(lineNode["drawType"].as<u8>());
        }
    }

    void InstanceLineSerializable::serialize(YAML::Emitter &out) {

    }

    void InstanceLineSerializable::deserialize(const YAML::Node &parent) {

    }

}
