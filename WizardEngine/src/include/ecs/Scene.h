//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "Family.h"

#include "set"

namespace engine {

    class Scene : public EntityContainer {

    public:
        Scene() = default;
        ~Scene() override {
            clear();
        }

    public:
        inline const uint32_t& getTextureId() const {
            return _textureId;
        }

        inline void setTextureId(const uint32_t &textureId) {
            _textureId = textureId;
        }

        inline std::vector<Family>& getFamilies() {
            return _families;
        }

//        inline void setFamilies(std::vector<Family>& families) {
//            _families = families;
//        }

    public:
        void addFamily(const Family &family);
        void removeFamily(const Family& family);
        void clear() override;
        bool isEmpty() override;
        bool isEmptyFamilies();

    private:
        uint32_t _textureId = 0; // id of texture that stores pixels of this scene
        std::vector<Family> _families; // list of all families

    };

    Ref<Scene> copy(Ref<Scene>& scene);

}