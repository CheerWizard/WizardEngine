//
// Created by mecha on 14.01.2022.
//

#pragma once

#include <WizardEngine.h>

class DLL_API Scriptable {

    public:
        Scriptable() = default;
        virtual ~Scriptable() = default;
    // lifecycle functions
    public:
        virtual void onCreate();
        virtual void onUpdate(Time dt);
        virtual void onDestroy();

    public:
        Entity entity;

    template<typename T, typename... Args>
    inline bool add(Args &&... args) {
        return entity.template add<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    inline bool update(Args &&... args) {
        return entity.template update<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    inline T& get() const {
        return entity.template get<T>();
    }

    template<typename T>
    [[nodiscard]] inline bool has() const {
        return entity.template has<T>();
    }

    template<typename T>
    [[nodiscard]] inline bool remove() const {
        return entity.template remove<T>();
    }
};