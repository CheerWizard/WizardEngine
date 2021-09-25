//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "Attribute.h"

#include "glm/glm.hpp"

#include "vector"

#define MIN_VERTEX_COUNT 3
#define MIN_OFFSET_IN_BUFFER 0
#define DEFAULT_COLOR {0.25f, 0.5f, 0.75f, 1}
#define DEFAULT_POSITION {0.5f, 0.5f, 0.5f}
#define DEFAULT_TEXTURE_COORDS {0.25f, -0.25f}

namespace engine {

    class VertexFormat {

    public:

        explicit VertexFormat() {
            _attributes = std::vector<Attribute>();
        }

        explicit VertexFormat(const std::vector<Attribute>& attributes) :
        _attributes(attributes) {}

        ~VertexFormat() {
            destroy();
        }

    public:
        inline std::vector<Attribute> getAttributes() const {
            return _attributes;
        }

    public:
        void destroy();

        uint32_t getElementCount() const;
        size_t getSize() const;

        uint32_t add(const Attribute &attribute); // returns index of new element.
        void replace(const uint32_t &index, const Attribute &attribute);
        const Attribute& get(const uint32_t &index) const;
        void clear();
        void remove(const uint32_t &index);

    private:
        std::vector<Attribute> _attributes;

    };

    // todo find a way to expand or hide data fields.
    struct Vertex {
        glm::vec3 position;
        glm::vec2 textureCoords;
        glm::vec4 color;

        Vertex(const glm::vec3 &position = DEFAULT_POSITION,
               const glm::vec2 &textureCoords = DEFAULT_TEXTURE_COORDS,
               const glm::vec4 &color = DEFAULT_COLOR) :
               position(position),
               color(color),
               textureCoords(textureCoords)
               {}
    };

    struct VertexData {
        Vertex* vertices;
        uint32_t vertexCount;
        uint32_t vertexStart;

        VertexData() = default;

        VertexData(Vertex* vertices,
                   const uint32_t &vertexStart = 0,
                   const uint32_t &vertexCount = MIN_VERTEX_COUNT) :
                   vertices(vertices),
                   vertexCount(vertexCount),
                   vertexStart(vertexStart) {}
    };

}