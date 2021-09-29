//
// Created by mecha on 29.09.2021.
//

#include "ObjFile.h"

namespace engine {

    const char *ObjFile::getExtensionName() const {
        return ".obj";
    }

    const char *ObjFile::getAssetPath() const {
        return OBJ_PATH;
    }

}