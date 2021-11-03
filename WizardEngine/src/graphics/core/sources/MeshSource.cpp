//
// Created by mecha on 22.10.2021.
//

#include "MeshSource.h"

namespace engine {

    bool MeshSource::exists(const std::string &name) {
        return _cache.find(name) != _cache.end();
    }

    void MeshSource::destroy() {
        _cache.clear();
    }

    const MeshComponent& MeshSource::getMesh(const std::string &fileName) {
        if (!exists(fileName)) {
            ENGINE_INFO("Obj file {0} data does not exists in cache! Reading data from .obj file!", fileName);
            _cache[fileName] = _objFile.readObj(fileName);
        }
        return _cache[fileName];
    }

    const MeshComponent& MeshSource::getTriangle(const std::string &name) {
        if (!exists(name)) {
            ENGINE_INFO("Triangle {0} does not exists in cache! Creating new triangle!", name);
            _cache[name] = Triangle();
        }
        return _cache[name];
    }

    const MeshComponent& MeshSource::getSquare(const std::string &name) {
        if (!exists(name)) {
            ENGINE_INFO("Square {0} does not exists in cache! Creating new square!", name);
            _cache[name] = Square();
        }
        return _cache[name];
    }

    const MeshComponent& MeshSource::getCube(const std::string &name) {
        if (!exists(name)) {
            ENGINE_INFO("Cube {0} does not exists in cache! Creating new cube!", name);
            _cache[name] = Cube();
        }
        return _cache[name];
    }

}