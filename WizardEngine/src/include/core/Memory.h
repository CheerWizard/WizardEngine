//
// Created by mecha on 29.08.2021.
//
#pragma once

#include <memory>

namespace engine::core {

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> createScope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> createRef(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Weak = std::weak_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Weak<T> createWeak(Args&& ... args) {
        return std::weak_ptr<T>(std::forward<Args>(args)...);
    }

}