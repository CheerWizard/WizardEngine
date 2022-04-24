//
// Created by mecha on 02.04.2022.
//

#pragma once

#include <cstdint>

namespace engine::core {
    // unsigned primitives
    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;
    // signed primitives
    using s8 = int8_t;
    using s16 = int16_t;
    using s32 = int32_t;
    using s64 = int64_t;

    using f32 = float;

    constexpr u32 kb_32 = 65536;

    constexpr u16 kb_16 = kb_32 / 2;
    constexpr u16 kb_8 = kb_16 / 2;
    constexpr u16 kb_4 = kb_8 / 2;
    constexpr u16 kb_2 = kb_4 / 2;
    constexpr u16 kb_1 = kb_2 / 2;

    constexpr u32 kb_64 = kb_32 * 2;
    constexpr u32 kb_128 = kb_64 * 2;
    constexpr u32 kb_256 = kb_128 * 2;
    constexpr u32 kb_512 = kb_256 * 2;
    constexpr u32 mb_1 = kb_512 * 2;

    enum class endian {
#ifdef _WIN32
        little = 0,
        big    = 1,
        native = little
#else
        little = __ORDER_LITTLE_ENDIAN__,
    big    = __ORDER_BIG_ENDIAN__,
    native = __BYTE_ORDER__
#endif
    };
}