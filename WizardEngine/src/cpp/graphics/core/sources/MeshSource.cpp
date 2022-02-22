//
// Created by mecha on 22.10.2021.
//

#include <graphics/core/sources/MeshSource.h>

namespace engine {

    Ref<MeshSource> MeshSource::instance = createRef<MeshSource>();

    bool MeshSource::exists(const std::string &name) {
        return _meshes.find(name) != _meshes.end();
    }

    void MeshSource::destroy() {
        _meshes.clear();
    }

    const MeshComponent& MeshSource::getMesh(const std::string &fileName) {
        if (!exists(fileName)) {
            ENGINE_INFO("Obj file {0} data does not exists in cache! Reading data from .obj file!", fileName);
            _meshes[fileName] = ObjFile::read(fileName);
        }
        return _meshes[fileName];
    }

    const MeshComponent& MeshSource::getTriangle(const std::string &name) {
        if (!exists(name)) {
            ENGINE_INFO("Triangle {0} does not exists in cache! Creating new triangle!", name);
            _meshes[name] = Shapes::newTriangle(name);
        }
        return _meshes[name];
    }

    const MeshComponent& MeshSource::getSquare(const std::string &name) {
        if (!exists(name)) {
            ENGINE_INFO("Square {0} does not exists in cache! Creating new square!", name);
            _meshes[name] = Shapes::newSquare(name);
        }
        return _meshes[name];
    }

    const MeshComponent& MeshSource::getCube(const std::string &name) {
        if (!exists(name)) {
            ENGINE_INFO("Cube {0} does not exists in cache! Creating new cube!", name);
            _meshes[name] = Shapes::newCube(name);
        }
        return _meshes[name];
    }

}