//
// Created by mecha on 07.09.2021.
//

#pragma once

#include "Attribute.h"

#include "vector"

namespace engine {

    class Vertex {

    public:

        explicit Vertex(const uint32_t &count = 1) : _count(count) {
            _attributes = std::vector<Attribute>();
        }

        explicit Vertex(const std::vector<Attribute>& attributes, const uint32_t &count = 1) :
        _count(count), _attributes(attributes) {}

        ~Vertex() {
            destroy();
        }

    public:
        inline void setCount(const uint32_t &count) {
            _count = count;
        }

        inline uint32_t getCount() const {
            return _count;
        }

        inline std::vector<Attribute> getAttributes() const {
            return _attributes;
        }

        inline void setAttributes(const std::vector<Attribute>& attributes) {
            _attributes = attributes;
        }

    public:
        void destroy();

        uint32_t getElementCount() const;
        size_t getSize() const;

        uint32_t add(const Attribute &attribute);
        void replace(const uint32_t &index, const Attribute &attribute);
        const Attribute& get(const uint32_t &index) const;
        void remove(const uint32_t &index);
        void clear();

    private:
        std::vector<Attribute> _attributes;
        uint32_t _count; // describes count of Vertex's.

    };

}