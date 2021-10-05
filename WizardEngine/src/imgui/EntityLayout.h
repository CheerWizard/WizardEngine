//
// Created by mecha on 03.10.2021.
//

#pragma once

#include "../ecs/Entity.h"
#include "../core/Layout.h"
#include "../core/String.h"

namespace engine {

    struct EntityLayoutProps {
        const char* name;
    };

    class EntityLayoutCallback {
    public:
        virtual void onCreateEntity() = 0;
        virtual void onRemoveEntity() = 0;
    };

    class EntityLayout : public Layout {

    public:
        EntityLayout(const EntityLayoutProps &entityLayoutProps,
                     EntityLayoutCallback* callback = nullptr) :
                _props(entityLayoutProps),
                _callback(callback),
                Layout() {}

        ~EntityLayout() {
            destroy();
        }

    public:
        void onUpdate(Time deltaTime) override;

    public:
        inline void setCallback(EntityLayoutCallback* callback) {
            _callback = callback;
        }

        inline void removeCallback() {
            _callback = nullptr;
        }

    private:
        void destroy();

    private:
        EntityLayoutProps _props;
        EntityLayoutCallback* _callback = nullptr;

    };

}