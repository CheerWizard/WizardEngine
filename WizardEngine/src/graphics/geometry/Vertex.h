//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "Attribute.h"

#include "vector"

namespace engine {

    class Vertex {

    public:

        explicit Vertex(const uint32_t &count = 1) : _count(count) {}

        explicit Vertex(const std::vector<Attribute*>& attributes, const uint32_t &count = 1) :
        _count(count), _attributes(attributes) {}

        ~Vertex() {
            destroy();
        }

    public:
        inline uint32_t getCount() const {
            return _count;
        }

        inline const std::vector<Attribute*> getAttributes() const {
            return _attributes;
        }

        inline void setAttributes(const std::vector<Attribute*>& attributes) {
            _attributes = attributes;
        }

    public:
        void destroy();

        uint32_t getElementCount() const;
        size_t getSize() const;

        uint32_t add(Attribute* attribute);
        void replace(const uint32_t &index, Attribute* attribute);
        Attribute& get(const uint32_t &index) const;
        void remove(const uint32_t &index);

    private:
        std::vector<Attribute*> _attributes;
        uint32_t _count; // count of Vertex copies.

    };

}