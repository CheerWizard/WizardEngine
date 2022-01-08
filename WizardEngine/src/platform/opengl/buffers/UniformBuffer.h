//
// Created by mecha on 02.10.2021.
//

#pragma once

#include "../../../graphics/core/buffer_data/UniformData.h"
#include "../../../graphics/core/shader/UniformBlockFormat.h"

namespace engine::shader {

    class UniformBuffer final {

    public:
        UniformBuffer() {
            create();
        }
        ~UniformBuffer() = default;

    public:
        [[nodiscard]] inline const UniformBlockFormat& getUniformBlockFormat() const {
            return uniformBlockFormat;
        }

        inline void setUniformBlockFormat(const UniformBlockFormat& uniformBlockFormat) {
            this->uniformBlockFormat = uniformBlockFormat;
        }

    public:
        // lifetime functions
        void create();
        void destroy() const;
        void recreate();
        // bind/unbind
        void bind() const;
        void unbind() const;
        void bindBlock() const;
        // GPU allocations
        void alloc() const;
        void alloc(const size_t &memorySize) const;
        void applyFormat(const UniformBlockFormat &uniformBlockFormat);
        // GPU data load
        void load(const UniformData &uniformData) const;

    private:
        uint32_t id = 0;
        UniformBlockFormat uniformBlockFormat;

    };

}