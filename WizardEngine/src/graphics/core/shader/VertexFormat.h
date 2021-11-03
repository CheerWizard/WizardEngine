//
// Created by mecha on 29.09.2021.
//

#pragma once

#include "VertexAttribute.h"

namespace engine {

    class VertexFormat {

    public:

        VertexFormat(const std::vector<VertexAttribute>& attributes = std::vector<VertexAttribute>()) :
        _attributes(attributes) {}

        ~VertexFormat() {
            destroy();
        }

    public:
        inline std::vector<VertexAttribute>& getAttributes() {
            return _attributes;
        }

    public:
        uint32_t getElementCount() const;
        size_t getSize() const;

        uint32_t add(const VertexAttribute &attribute); // returns index of new element.
        void replace(const uint32_t &index, const VertexAttribute &attribute);
        const VertexAttribute& get(const uint32_t &index) const;
        void clear();
        void remove(const uint32_t &index);
        bool isEmpty();

    private:
        void destroy();

    private:
        std::vector<VertexAttribute> _attributes;

    };

}