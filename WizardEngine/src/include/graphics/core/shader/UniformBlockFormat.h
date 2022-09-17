//
// Created by mecha on 02.10.2021.
//

#pragma once

#include <graphics/core/shader/UniformAttribute.h>

namespace engine::shader {

    class ENGINE_API UniformBlockFormat final {

    public:
        UniformBlockFormat() = default;
        UniformBlockFormat(const u32& id) : _id(id) {}
        ~UniformBlockFormat();

    public:
        inline std::vector<UniformAttribute>& getAttributes() {
            return _attributes;
        }

        [[nodiscard]] inline const std::string& getName() const {
            return _name;
        }

        inline void setName(const std::string &name) {
            _name = name;
        }

        [[nodiscard]] inline const uint32_t& getId() const {
            return _id;
        }

    public:
        [[nodiscard]] uint32_t getElementCount() const;
        [[nodiscard]] size_t size() const;
        [[nodiscard]] uint32_t count() const;

        uint32_t add(UniformAttribute &attribute); // returns index of new element.
        void replace(const uint32_t &index, const UniformAttribute &attribute);
        [[nodiscard]] const UniformAttribute& get(const uint32_t &index) const;
        void clear();
        void remove(const uint32_t &index);
        [[nodiscard]] bool isEmpty() const;
        void add(std::vector<UniformAttribute> &attributes);

    public:
        const UniformAttribute& operator [](const u32 &index) const {
            return _attributes[index];
        }

    private:
        u32 _id = 0;
        std::string _name;
        vector<UniformAttribute> _attributes;

    };

}