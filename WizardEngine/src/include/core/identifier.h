//
// Created by mecha on 02.04.2022.
//

#pragma once

#include <core/primitives.h>
#include <core/Assert.h>
#include <typeinfo>

namespace engine::core {

    constexpr u8 u8_invalid_id { 0xffui8 }; // -1 in 8-bit
    constexpr u16 u16_invalid_id { 0xffffui16 }; // -1 in 16-bit
    constexpr u32 u32_invalid_id { 0xffff'ffffui32 }; // -1 in 32-bit
    constexpr u64 u64_invalid_id { 0xffff'ffff'ffff'ffffui64 }; // -1 in 64-bit

    using id_type = u32;

    // should be used as private namespace, in scope of engine
    namespace internal {
        constexpr u32 generation_bit { 8 };
        constexpr u32 index_bit { sizeof(id_type) * 8 - generation_bit };
        constexpr id_type index_mask { (id_type { 1 } << index_bit) - 1 };
        constexpr id_type generation_mask { (id_type { 1 } << generation_bit) - 1 };
    }

    constexpr id_type invalid_id { id_type(-1) };

    using generation_type = std::conditional_t<internal::generation_bit <= 16, std::conditional_t<internal::generation_bit <= 8, u8, u16>, u32>;
    // compilation tests
    static_assert(sizeof(generation_type) * 8 >= internal::generation_bit);
    static_assert((sizeof(id_type) - sizeof(generation_type)) > 0);

    // util functions
    constexpr bool is_valid(id_type id) {
        return id != invalid_id;
    }

    constexpr id_type index(id_type id) {
        id_type index { id & internal::index_mask };
        ENGINE_ASSERT(index != internal::index_mask, "Identifier: invalid index for id: {0}", id);
        return index;
    }

    constexpr id_type generation(id_type id) {
        return (id >> internal::index_bit) & internal::generation_mask;
    }

    constexpr id_type new_generation(id_type id) {
        const id_type generation { core::generation(id) + 1 };
        ENGINE_ASSERT(generation < (((u64)1 << internal::generation_bit) - 1),
                      "Identifier: generation number is out of range. generation: {0}, max: {1}", generation, 255);
        return index(id) | (generation << internal::index_bit);
    }

#ifdef DEBUG
namespace internal {

    struct base_id {
        constexpr explicit base_id(id_type id) : _id(id) {}
        constexpr explicit operator id_type() const { return _id; }
    private:
        id_type _id;
    };

#define DEFINE_TYPED_ID(type_name)                                        \
            struct type_name final : core::internal::base_id {                  \
                constexpr explicit type_name(core::id_type id)                       \
                : core::internal::base_id(id) {}                                \
                constexpr type_name() : core::internal::base_id { 0 } {}             \
            };
}
#else
#define DEFINE_TYPED_ID(name) using name = id::id_type;
#endif
}