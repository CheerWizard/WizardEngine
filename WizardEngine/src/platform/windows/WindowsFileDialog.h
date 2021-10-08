//
// Created by mecha on 08.10.2021.
//

#pragma once

#include "../../core/FileDialog.h"

namespace engine {

    class WindowsFileDialog : public FileDialog {

    public:
        WindowsFileDialog(void* nativeWindow) : FileDialog(nativeWindow) {}
        ~WindowsFileDialog() override = default;

        std::string getImportPath(const char *filter) override;
        std::string getExportPath(const char *filter) override;

    };

}