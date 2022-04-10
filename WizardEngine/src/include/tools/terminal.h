//
// Created by mecha on 09.10.2021.
//

#pragma once

#include <thread/Task.h>

namespace engine::terminal {
    void openNotepad(const std::string &filePath);
    void openVSCode(const std::string &filePath);
    void openVisualStudio(const std::string &filePath);
    void openPhotoshop(const std::string &filePath);
    void openBlender(const std::string &filePath);
    void openZBrush(const std::string &filePath);
    void msBuild(const std::string& slnPath);
    void cmake(const std::string& cmakePath);
    void openNotepadTask(const std::string &filePath);
    void openVSCodeTask(const std::string &filePath);
    void openVisualStudioTask(const std::string &filePath);
    void openPhotoshopTask(const std::string &filePath);
    void openBlenderTask(const std::string &filePath);
    void openZBrushTask(const std::string &filePath);
    void msBuildTask(const std::string &slnPath);
    void cmakeTask(const std::string& cmakePath);
    void cmake(const std::string& cmakePath, const std::vector<const char*>& definitions);
}