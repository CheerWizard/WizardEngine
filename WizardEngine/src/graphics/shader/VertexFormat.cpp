//
// Created by mecha on 29.09.2021.
//

#include "VertexFormat.h"

namespace engine {

    const Attribute& VertexFormat::get(const uint32_t &index) const {
        return _attributes[index];
    }

    void VertexFormat::destroy() {
        clear();
    }

    uint32_t VertexFormat::getElementCount() const {
        uint32_t elementCount = 0;
        for (const auto &attribute : _attributes) {
            elementCount += attribute.elementCount;
        }
        return elementCount;
    }

    size_t VertexFormat::getSize() const {
        size_t size = 0;
        for (const auto &attribute : _attributes) {
            size += attribute.elementCount * sizeof(float);
        }
        return size;
    }

    void VertexFormat::clear() {
        _attributes.clear();
    }

    void VertexFormat::replace(const uint32_t &index, const Attribute &attribute) {
        _attributes.emplace(_attributes.begin() + index, attribute);
    }

    uint32_t VertexFormat::add(const Attribute &attribute) {
        _attributes.emplace_back(attribute);
        return _attributes.size() - 1;
    }

    void VertexFormat::remove(const uint32_t &index) {
        _attributes.erase(_attributes.begin() + index);
    }

}