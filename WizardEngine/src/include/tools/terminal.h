//
// Created by mecha on 09.10.2021.
//

#pragma once

#include <thread/Task.h>

namespace engine::terminal {
    ENGINE_API void openNotepad(const std::string &filePath);
    ENGINE_API void openVSCode(const std::string &filePath);
    ENGINE_API void openVisualStudio(const std::string &filePath);
    ENGINE_API void openPhotoshop(const std::string &filePath);
    ENGINE_API void openBlender(const std::string &filePath);
    ENGINE_API void openZBrush(const std::string &filePath);
    ENGINE_API void msBuild(const std::string& slnPath);
    ENGINE_API void cmake(const std::string& cmakePath);
    ENGINE_API void cmakeD(const std::string& cmakePath, const std::vector<const char*>& definitions);
    ENGINE_API void exe(const std::string& exePath);

    ENGINE_API void openNotepadTask(const std::string &filePath);
    ENGINE_API void openVSCodeTask(const std::string &filePath);
    ENGINE_API void openVisualStudioTask(const std::string &filePath);
    ENGINE_API void openPhotoshopTask(const std::string &filePath);
    ENGINE_API void openBlenderTask(const std::string &filePath);
    ENGINE_API void openZBrushTask(const std::string &filePath);
    ENGINE_API void msBuildTask(const std::string &slnPath);
    ENGINE_API void cmakeTask(const std::string& cmakePath);
}