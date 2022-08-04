//
// Created by mecha on 31.05.2022.
//

#pragma once

#include <xhash>
#include <core/primitives.h>

namespace engine {

    using namespace core;
    // "UUID" (universally unique identifier) or GUID is (usually) a 128-bit integer
    // used to "uniquely" identify information.
    class uuid {

    public:
        uuid();
        uuid(u64 uuid);
        uuid(const uuid& other);

    public:
        operator u64() { return _uuid; }
        operator const u64() const { return _uuid; }
        bool operator ==(u64 other) const { return _uuid == other; }

    private:
        u64 _uuid;
    };
}

namespace std {

    template <>
    struct hash<engine::uuid> {
        std::size_t operator()(const engine::uuid& uuid) const {
            return hash<engine::core::u64>()((engine::core::u64)uuid);
        }
    };

}
