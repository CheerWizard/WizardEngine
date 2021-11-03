//
// Created by mecha on 02.10.2021.
//

#include "UniformBlockFormat.h"

namespace engine {

    const UniformAttribute& UniformBlockFormat::get(const uint32_t &index) const {
        return _attributes[index];
    }

    void UniformBlockFormat::destroy() {
        clear();
    }

    uint32_t UniformBlockFormat::getElementCount() const {
        uint32_t elementCount = 0;
        for (const auto &attribute : _attributes) {
            elementCount += attribute.elementCount;
        }
        return elementCount;
    }

    size_t UniformBlockFormat::size() const {
        size_t size = 0;
        for (const auto &attribute : _attributes) {
            size += attribute.size();
        }
        return size;
    }

    void UniformBlockFormat::clear() {
        _attributes.clear();
    }

    void UniformBlockFormat::replace(const uint32_t &index, const UniformAttribute &attribute) {
        _attributes.emplace(_attributes.begin() + index, attribute);
    }

    uint32_t UniformBlockFormat::add(const UniformAttribute &attribute) {
        _attributes.emplace_back(attribute);
        return _attributes.size() - 1;
    }

    void UniformBlockFormat::remove(const uint32_t &index) {
        _attributes.erase(_attributes.begin() + index);
    }

    bool UniformBlockFormat::isEmpty() {
        return _attributes.empty();
    }

}