//
// Created by mecha on 22.10.2021.
//

#pragma once

#include <graphics/core/io/ModelFile.h>
#include <graphics/core/geometry/Shapes.h>
#include <graphics/core/RenderModel.h>

#include "unordered_map"

namespace engine::graphics {

    template<typename T>
    class MeshSource final {

        typedef std::unordered_map<std::string, BaseMeshComponent<T>> Meshes;

    private:
        MeshSource() = default;

    public:
        static MeshSource& get() {
            static MeshSource<T> instance;
            return instance;
        }

    public:
        void clear();
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

    private:
        Meshes _meshes;
    };

    template<typename T>
    bool MeshSource<T>::exists(const std::string &name) {
        return _meshes.find(name) != _meshes.end();
    }

    template<typename T>
    const BaseMeshComponent<T>& MeshSource<T>::getMesh(const std::string &fileName) {
        auto model = io::ModelFile::read(fileName);
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

    template<typename T>
    void MeshSource<T>::clear() {
        _meshes.clear();
    }
}
