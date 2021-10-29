//
// Created by mecha on 22.10.2021.
//

#pragma once

#include "obj/ObjFile.h"
#include "../geometry/Shapes.h"

#include "unordered_map"

namespace engine {

    class MeshSource {

    public:
        MeshSource() = default;
        ~MeshSource() {
            destroy();
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
        std::unordered_map<std::string, MeshComponent> _cache;
        ObjFile _objFile;

    };

}
