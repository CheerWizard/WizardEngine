//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <algorithm>

#define MIN_VERTEX_COUNT 3

namespace engine {

    template<typename V>
    struct BatchVertex {
        V vertex;
        float id = 0;
    };

    template<typename V>
    struct InstanceVertex {
        V vertex;
    };

    template<typename V>
    struct VertexData {
        V* vertices = nullptr;
        uint32_t vertexStart = 0;
        uint32_t vertexCount = MIN_VERTEX_COUNT;
    };

    enum DrawType {
        QUAD, TRIANGLE, TRIANGLE_STRIP,
        LINE, LINE_STRIP, LINE_LOOP
    };

    template<typename T>
    struct VertexDataComponent {
        VertexData<T> vertexData;
        bool isUpdated = true;
        uint32_t renderModelId = 0;
        DrawType drawType = QUAD;
    };

    template<typename T>
    VertexData<T> copy(const VertexData<T> &vertexData) {
        auto* copyVertices = new T[vertexData.vertexCount];
        std::copy(vertexData.vertices, vertexData.vertices + vertexData.vertexCount, copyVertices);

        return VertexData<T> {
                copyVertices,
                vertexData.vertexStart,
                vertexData.vertexCount
        };
    }

    template<typename T>
    VertexDataComponent<T> copy(const VertexDataComponent<T> &vertexDataComponent) {
        return { copy(vertexDataComponent.vertexData) };
    }

    template<typename T>
    void setBatchId(VertexDataComponent<BatchVertex<T>>& vertexDataComponent, const uint32_t& batchId) {
        auto& vertexData = vertexDataComponent.vertexData;
        for (auto i = 0; i < vertexData.vertexCount; i++) {
            vertexData.vertices[i].id = (float) batchId;
        }
    }

    template<typename T>
    void updateStart(VertexDataComponent<T> &vertexDataComponent, const uint32_t &prevVertexCount) {
        if (vertexDataComponent.vertexData.vertexStart != prevVertexCount) {
            vertexDataComponent.vertexData.vertexStart = prevVertexCount;
        }
    }

    template<typename T>
    void invalidate(VertexDataComponent<T> &vertexDataComponent) {
        vertexDataComponent.vertexData.vertexStart = 0;
    }

    template<typename IN, typename OUT>
    VertexData<OUT> toVertexData(const std::vector<IN>& inVertices) {
        size_t size = inVertices.size();
        auto* outVertices = new OUT[size];
        for (auto i = 0; i < size; i++) {
            outVertices[i] = { inVertices[i] };
        }
        return VertexData<OUT> {
                outVertices,
                0,
                static_cast<uint32_t>(size)
        };
    }

    template<typename IN, typename OUT, size_t Size>
    VertexData<OUT> toVertexData(const std::array<IN, Size>& inVertices) {
        auto* outVertices = new OUT[Size];
        for (auto i = 0; i < Size; i++) {
            outVertices[i] = { inVertices[i] };
        }
        return VertexData<OUT> {
                outVertices,
                0,
                static_cast<uint32_t>(Size)
        };
    }
}
