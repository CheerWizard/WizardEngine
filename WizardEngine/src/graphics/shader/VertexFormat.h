//
// Created by mecha on 29.09.2021.
//

#pragma once

#include "Attribute.h"

#include "vector"

#define MIN_VERTEX_COUNT 3
#define MIN_OFFSET_IN_BUFFER 0

namespace engine {

    class VertexFormat {

    public:

        VertexFormat(const std::vector<Attribute>& attributes = std::vector<Attribute>()) :
        _attributes(attributes) {}

        ~VertexFormat() {
            destroy();
        }

    public:
        inline std::vector<Attribute>& getAttributes() {
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
        bool isEmpty();

    private:
        std::vector<Attribute> _attributes;

    };

}