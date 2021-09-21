//
// Created by mecha on 07.09.2021.
//

#include "Vertex.h"

namespace engine {

    Attribute& Vertex::get(const uint32_t &index) const {
        return *_attributes[index];
    }

    void Vertex::remove(const uint32_t &index) {
        _attributes.erase(_attributes.begin() + index);
    }

    void Vertex::destroy() {
        _attributes.clear();
        _count = 0;
    }

    uint32_t Vertex::add(Attribute* attribute) {
        _attributes.emplace_back(attribute);
        return _attributes.size() - 1;
    }

    void Vertex::replace(const uint32_t &index, Attribute* attribute) {
        _attributes.emplace(_attributes.begin() + index, attribute);
    }

    uint32_t Vertex::getElementCount() const {
        uint32_t elementCount = 0;
        for (const auto& attribute : _attributes) {
            elementCount += attribute->elementCount;
        }
        return elementCount;
    }

    size_t Vertex::getSize() const {
        size_t size = 0;
        for (const auto& attribute : _attributes) {
            size += attribute->elementCount * sizeof(float);
        }
        return size;
    }

}