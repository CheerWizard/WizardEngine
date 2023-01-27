//
// Created by mecha on 29.09.2021.
//

#include <graphics/core/shader/VertexFormat.h>

namespace engine::shader {

    const VertexAttribute& VertexFormat::get(const uint32_t &index) const {
        return _attributes[index];
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
            size += elementSize(attribute);
        }
        return size;
    }

    void VertexFormat::clear() {
        _attributes.clear();
    }

    void VertexFormat::replace(const uint32_t &index, const VertexAttribute &attribute) {
        _attributes.emplace(_attributes.begin() + index, attribute);
    }

    uint32_t VertexFormat::add(const VertexAttribute &attribute) {
        _attributes.emplace_back(attribute);
        return _attributes.size() - 1;
    }

    void VertexFormat::remove(const uint32_t &index) {
        _attributes.erase(_attributes.begin() + index);
    }

    bool VertexFormat::isEmpty() {
        return _attributes.empty();
    }

    void VertexFormat::setAttrCategory(u8 attributeCategory) {
        for (auto& attribute : _attributes) {
            attribute.category = attributeCategory;
        }
    }
}
