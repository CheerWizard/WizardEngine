//
// Created by mecha on 22.10.2021.
//

#pragma once

#include <graphics/core/io/ObjFile.h>
#include <graphics/core/geometry/Shapes.h>
#include <graphics/core/RenderModel.h>

#include "unordered_map"

#define GET_MESH(objName) engine::MeshSource::get()->getMesh(objName)

namespace engine {

    class MeshSource {
        // key - .obj file name or mesh name
        // value - actual mesh loaded from .obj file or constructed directly
        typedef std::unordered_map<std::string, MeshComponent> Meshes;
        // key - mesh component
        // value - actual mesh loaded from .obj file or constructed directly
        typedef std::unordered_map<std::string, RenderModel> RenderModels;

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

        Meshes _meshes;
        RenderModels _renderModels;

    private:
        static Ref<MeshSource> instance;
    };
}
