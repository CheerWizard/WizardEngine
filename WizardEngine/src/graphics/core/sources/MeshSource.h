//
// Created by mecha on 22.10.2021.
//

#pragma once

#include "../io/ObjFile.h"
#include "../geometry/Shapes.h"

#include "unordered_map"

#define GET_MESH(objName) engine::MeshSource::get()->getMesh(objName)

namespace engine {

    class MeshSource {

    public:
        MeshSource() = default;
        ~MeshSource() {
            destroy();
        }

    public:
        static const Ref<MeshSource>& get() {
            return instance;
        }

    public:
        /**
         * @return MeshComponent from either cache or .obj file.
         */
        const MeshComponent& getMesh(const std::string &fileName);

        /**
         * @return cached MeshComponent that represents triangle shape.
         */
         const MeshComponent& getTriangle(const std::string &name);

         /**
          * @return cached MeshComponent that represents square shape.
          */
         const MeshComponent& getSquare(const std::string &name);

         /**
          * @return cached MeshComponent that represents cube shape.
          */
         const MeshComponent& getCube(const std::string &name);

    private:
        bool exists(const std::string &name);
        void destroy();

    private:
        // key - .obj file name or mesh name
        // value - actual mesh loaded from .obj file or constructed directly
        std::unordered_map<std::string, MeshComponent> _cache;

    private:
        static Ref<MeshSource> instance;
    };
}
