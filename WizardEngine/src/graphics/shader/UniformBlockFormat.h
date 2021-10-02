//
// Created by mecha on 02.10.2021.
//

#pragma once

#include "UniformAttribute.h"

namespace engine {

    class UniformBlockFormat {

    public:

        UniformBlockFormat(
                const std::string& name = "",
                const std::vector<UniformAttribute>& attributes = std::vector<UniformAttribute>()) :
        _attributes(attributes),
        _name(name) {}

        ~UniformBlockFormat() {
            destroy();
        }

    public:
        inline std::vector<UniformAttribute>& getAttributes() {
            return _attributes;
        }

        inline const std::string getName() {
            return _name;
        }

        inline void setName(const std::string &name) {
            _name = name;
        }

    public:
        void destroy();

        uint32_t getElementCount() const;
        size_t getSize() const;

        uint32_t add(const UniformAttribute &attribute); // returns index of new element.
        void replace(const uint32_t &index, const UniformAttribute &attribute);
        const UniformAttribute& get(const uint32_t &index) const;
        void clear();
        void remove(const uint32_t &index);
        bool isEmpty();

    private:
        std::string _name;
        std::vector<UniformAttribute> _attributes;

    };

}