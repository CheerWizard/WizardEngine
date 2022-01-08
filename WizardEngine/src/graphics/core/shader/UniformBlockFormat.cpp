//
// Created by mecha on 02.10.2021.
//

#include "UniformBlockFormat.h"

#include "../../../core/Logger.h"

namespace engine::shader {

    const UniformAttribute& UniformBlockFormat::get(const uint32_t &index) const {
        return _attributes[index];
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
            size += size_of(attribute);
        }
        return size;
    }

    void UniformBlockFormat::clear() {
        _attributes.clear();
    }

    void UniformBlockFormat::replace(const uint32_t &index, const UniformAttribute &attribute) {
        _attributes.emplace(_attributes.begin() + index, attribute);
    }

    uint32_t UniformBlockFormat::add(UniformAttribute &attribute) {
        ENGINE_INFO(
                "Adding new uniform block attribute {0} {1}, block name : {2}",
                attribute.elementCount,
                attribute.name,
                _name
        );
        attribute.offset = size();
        _attributes.emplace_back(attribute);
        return _attributes.size() - 1;
    }

    void UniformBlockFormat::remove(const uint32_t &index) {
        _attributes.erase(_attributes.begin() + index);
    }

    bool UniformBlockFormat::isEmpty() const {
        return _attributes.empty();
    }

    void UniformBlockFormat::add(std::vector<UniformAttribute> &attributes) {
        for (auto& attribute : attributes) {
            add(attribute);
        }
    }

    uint32_t UniformBlockFormat::count() const {
        return _attributes.size();
    }

    UniformBlockFormat::~UniformBlockFormat() {
        clear();
    }
}