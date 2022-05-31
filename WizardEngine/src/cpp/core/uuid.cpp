//
// Created by mecha on 31.05.2022.
//

#include <core/uuid.h>
#include <random>

namespace engine {

    static std::random_device randomDevice;
    static std::mt19937_64 mtEngine(randomDevice());
    static std::uniform_int_distribution<uint64_t> uniformDistribution;

    uuid::uuid() : _uuid(uniformDistribution(mtEngine)) {}
    uuid::uuid(uint64_t uuid) : _uuid(uuid) {}
    uuid::uuid(const uuid& other) = default;

}