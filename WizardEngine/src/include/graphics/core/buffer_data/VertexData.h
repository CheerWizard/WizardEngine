//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <ecs/ecs.h>
#include <algorithm>
#include <functional>

#define MIN_VERTEX_COUNT 3

namespace engine::graphics {

    using namespace core;

    template<typename V>
    struct BatchVertex {
        V vertex;
        f32 id = 0;
    };

    template<typename V>
    struct InstanceVertex {
        V vertex;
    };

    enum class DrawType : u16 {
        QUAD = 0, TRIANGLE = 1, TRIANGLE_STRIP = 2,
        LINE = 3, LINE_STRIP = 4, LINE_LOOP = 5,
        POINTS = 6
    };

    template_component(VertexDataComponent, T) {
        array<T> vertexData;
        bool isUpdated = true;
        u8 renderModelId = 0;
        DrawType drawType = DrawType::QUAD;
    };

    template<typename T>
    array<T> copy(const array<T> &vertexData) {
        auto* copyVertices = new T[vertexData.size];
        std::copy(vertexData.values, vertexData.values + vertexData.size, copyVertices);

        return array<T> {
                copyVertices,
                vertexData.offset,
                vertexData.size
        };
    }

    template<typename T>
    VertexDataComponent<T> copy(const VertexDataComponent<T> &vertexDataComponent) {
        return { copy(vertexDataComponent.vertexData) };
    }

    template<typename T>
    void setBatchId(VertexDataComponent<BatchVertex<T>>& vertexDataComponent, const uint32_t& batchId) {
        auto& vertexData = vertexDataComponent.vertexData;
        for (auto i = 0; i < vertexData.size; i++) {
            vertexData.values[i].id = (float) batchId;
        }
    }

    template<typename T>
    void updateStart(VertexDataComponent<T> &vertexDataComponent, const uint32_t &prevVertexCount) {
        vertexDataComponent.vertexData.offset = prevVertexCount;
    }

    template<typename T>
    void invalidate(VertexDataComponent<T> &vertexDataComponent) {
        vertexDataComponent.vertexData.offset = 0;
    }

    template<typename IN, typename OUT>
    array<OUT> toVertexData(const std::vector<IN>& inVertices) {
        size_t size = inVertices.size();
        auto* outVertices = new OUT[size];
        for (auto i = 0; i < size; i++) {
            outVertices[i] = { inVertices[i] };
        }
        return array<OUT> {
                outVertices,
                0,
                static_cast<uint32_t>(size)
        };
    }

    template<typename FROM, typename TO>
    array<TO> toVertexData(const array<FROM>& fromVertexData, const std::function<TO(const FROM&)> vertexMapper) {
        auto* fromVertices = fromVertexData.values;
        auto* toVertices = new TO[fromVertexData.size];
        for (auto i = 0; i < fromVertexData.size; i++) {
            toVertices[i] = vertexMapper(fromVertices[i]);
        }
        return array<TO> {
                toVertices,
                fromVertexData.offset,
                fromVertexData.size
        };
    }

    template<typename FROM, typename TO>
    VertexDataComponent<TO> toVertexDataComponent(
            const VertexDataComponent<FROM>& fromVertexDataComponent,
            const std::function<TO(const FROM&)> vertexMapper
    ) {
        return VertexDataComponent<TO>{ toVertexData<FROM, TO>(fromVertexDataComponent.vertexData, vertexMapper) };
    }
}

namespace YAML {

    using namespace engine::graphics;

    template<typename V>
    struct convert<InstanceVertex<V>> {

        static Node encode(const InstanceVertex<V>& instanceVertex) {
            Node node;
            instanceVertex.vertex.encode(node);
            return node;
        }

        static bool decode(const Node& node, InstanceVertex<V>& instanceVertex) {
            instanceVertex.vertex.decode(node, 0);
            return true;
        }

    };

    template<typename V>
    struct convert<BatchVertex<V>> {

        static Node encode(const BatchVertex<V>& batchVertex) {
            Node node;
            node.push_back(batchVertex.id);
            batchVertex.vertex.encode(node);
            return node;
        }

        static bool decode(const Node& node, BatchVertex<V>& batchVertex) {
            batchVertex.id = node[0].as<f32>();
            batchVertex.vertex.decode(node, 1);
            return true;
        }

    };
}