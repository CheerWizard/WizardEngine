//
// Created by mecha on 09.10.2021.
//

#pragma once

#include <thread/Task.h>

namespace engine::tools {

    static void openNotepad(const std::string &filePath);
    static void openVSCode(const std::string &filePath);
    static void openVisualStudio(const std::string &filePath);
    static void openPhotoshop(const std::string &filePath);
    static void openBlender(const std::string &filePath);
    static void openZBrush(const std::string &filePath);
    static void msBuild(const std::string& slnPath);
    static void cmake(const std::string& cmakePath);
    static void openNotepadTask(const std::string &filePath);
    static void openVSCodeTask(const std::string &filePath);
    static void openVisualStudioTask(const std::string &filePath);
    static void openPhotoshopTask(const std::string &filePath);
    static void openBlenderTask(const std::string &filePath);
    static void openZBrushTask(const std::string &filePath);
    static void msBuildTask(const std::string &slnPath);
    static void cmakeTask(const std::string& cmakePath);
    static void cmake(const std::string& cmakePath, const std::vector<const char*>& definitions);

}