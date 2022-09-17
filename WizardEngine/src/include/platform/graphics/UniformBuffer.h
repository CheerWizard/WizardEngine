//
// Created by mecha on 02.10.2021.
//

#pragma once

#include <graphics/core/buffer_data/UniformData.h>
#include <graphics/core/shader/UniformBlockFormat.h>

namespace engine::shader {

    class ENGINE_API UniformBuffer final {

    public:
        UniformBuffer() = default;
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
        static void unbind();
        void bindBlock() const;
        // GPU allocations
        void alloc() const;
        static void alloc(const size_t &memorySize);
        void applyFormat(const UniformBlockFormat &uniformBlockFormat);
        // GPU data load
        void load(const UniformData &uniformData) const;

    private:
        u32 id = 0;
        UniformBlockFormat uniformBlockFormat;

    };

}