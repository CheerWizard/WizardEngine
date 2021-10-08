//
// Created by mecha on 08.10.2021.
//

#pragma once

#include "String.h"

namespace engine {

    class FileDialog {

    public:
        FileDialog(void* nativeWindow) : nativeWindow(nativeWindow) {}

        virtual ~FileDialog() {
            nativeWindow = nullptr;
        }

    public:
        virtual std::string getImportPath(const char* filter) = 0;
        virtual std::string getExportPath(const char* filter) = 0;

    protected:
        void* nativeWindow;

    };

}