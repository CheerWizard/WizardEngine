//
// Created by mecha on 08.10.2021.
//

#pragma once

#include <core/String.h>

namespace engine::tools {

    class FileDialog {

    public:
        FileDialog(void* nativeWindow) : nativeWindow(nativeWindow) {}

        ~FileDialog() {
            nativeWindow = nullptr;
        }

    public:
        std::string getImportPath(const char* filter);
        std::string getExportPath(const char* filter);

    private:
        void* nativeWindow;
    };

}