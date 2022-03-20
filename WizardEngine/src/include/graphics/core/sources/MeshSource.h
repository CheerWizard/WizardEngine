//
// Created by mecha on 22.10.2021.
//

#pragma once

#include <graphics/core/io/ObjFile.h>
#include <graphics/core/geometry/Shapes.h>
#include <graphics/core/RenderModel.h>

#include "unordered_map"

#define GET_BASE_MESH(T, objName) engine::MeshSource<T>::get()->getMesh(objName)
#define GET_OBJ(objName) GET_BASE_MESH(ObjVertex, objName)
// for default (batched) rendering
#define GET_OBJ_MESH(T, objName) toMeshComponent<InstanceVertex<T>>(GET_OBJ(objName))
// for instanced rendering
#define GET_OBJ_MESH_INSTANCED(T, objName) toMeshComponent<T>(GET_OBJ(objName))

namespace engine {

    template<typename T>
    class MeshSource {
        typedef std::unordered_map<std::string, BaseMeshComponent<T>> Meshes;

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
        const BaseMeshComponent<T>& getMesh(const std::string &fileName);

        /**
         * @return cached MeshComponent that represents triangle shape.
         */
         const BaseMeshComponent<T>& getTriangle(const std::string &name);

         /**
          * @return cached MeshComponent that represents square shape.
          */
         const BaseMeshComponent<T>& getSquare(const std::string &name);

         /**
          * @return cached MeshComponent that represents cube shape.
          */
         const BaseMeshComponent<T>& getCube(const std::string &name);

    private:
        bool exists(const std::string &name);
        void destroy();

    private:
        Meshes _meshes;

    private:
        static Ref<MeshSource> instance;
    };

    template<typename T>
    Ref<MeshSource<T>> MeshSource<T>::instance = createRef<MeshSource>();

    template<typename T>
    bool MeshSource<T>::exists(const std::string &name) {
        return _meshes.find(name) != _meshes.end();
    }

    template<typename T>
    void MeshSource<T>::destroy() {
        _meshes.clear();
    }

    template<typename T>
    const BaseMeshComponent<T>& MeshSource<T>::getMesh(const std::string &fileName) {
        if (!exists(fileName)) {
            ENGINE_INFO("Obj file {0} data does not exists in cache! Reading data from .obj file!", fileName);
            _meshes[fileName] = ObjFile::read(fileName);
        }
        return _meshes[fileName];
    }

    template<typename T>
    const BaseMeshComponent<T>& MeshSource<T>::getTriangle(const std::string &name) {
        if (!exists(name)) {
            ENGINE_INFO("Triangle {0} does not exists in cache! Creating new triangle!", name);
            _meshes[name] = Shapes<T>::newTriangle();
        }
        return _meshes[name];
    }

    template<typename T>
    const BaseMeshComponent<T>& MeshSource<T>::getSquare(const std::string &name) {
        if (!exists(name)) {
            ENGINE_INFO("Square {0} does not exists in cache! Creating new square!", name);
            _meshes[name] = Shapes<T>::newSquare();
        }
        return _meshes[name];
    }

    template<typename T>
    const BaseMeshComponent<T>& MeshSource<T>::getCube(const std::string &name) {
        if (!exists(name)) {
            ENGINE_INFO("Cube {0} does not exists in cache! Creating new cube!", name);
            _meshes[name] = Shapes<T>::newCube();
        }
        return _meshes[name];
    }
}
