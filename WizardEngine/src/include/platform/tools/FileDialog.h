//
// Created by mecha on 08.10.2021.
//

#pragma once

#include <core/String.h>
#include <core/filesystem.h>

namespace engine::tools {

    class ENGINE_API FileDialog {

    public:
        FileDialog(void* nativeWindow) : nativeWindow(nativeWindow) {}

        ~FileDialog() {
            nativeWindow = nullptr;
        }

    public:
        std::string getImportPath(const char* filter, const char* initialDir = CURRENT_WORKING_DIR.c_str());
        std::string getExportPath(const char* filter, const char* initialDir = CURRENT_WORKING_DIR.c_str());

    private:
        void* nativeWindow;
    };

}