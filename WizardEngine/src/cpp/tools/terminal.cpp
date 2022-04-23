//
// Created by mecha on 09.10.2021.
//

#include <tools/terminal.h>
#include <sstream>

namespace engine::terminal {

    void openVSCodeTask(const std::string &filePath) {
        EDITOR_INFO("openVSCodeTask({0})", filePath);
        thread::VoidTask<const std::string&> task = {
                "VSCode_Task",
                "VSCode_Thread",
                openVSCode
        };
        task.run(filePath);
    }

    void openVSCode(const std::string &filePath) {
        EDITOR_INFO("openVSCode({0})", filePath);
        std::string command = "code " + filePath;
        system(command.c_str());
    }

    void openNotepadTask(const std::string &filePath) {
        EDITOR_INFO("openNotepadTask({0})", filePath);
        thread::VoidTask<const std::string&> task = {
                "VSCode_Task",
                "VSCode_Thread",
                openNotepad
        };
        task.run(filePath);
    }

    void openNotepad(const std::string &filePath) {
        EDITOR_INFO("openNotepad({0})", filePath);
        std::string command = "notepad " + filePath;
        system(command.c_str());
    }

    void openVisualStudioTask(const std::string &filePath) {
        EDITOR_INFO("openVisualStudioTask({0})", filePath);
        thread::VoidTask<const std::string&> task = {
                "VSCode_Task",
                "VSCode_Thread",
                openVisualStudio
        };
        task.run(filePath);
    }

    void openVisualStudio(const std::string &filePath) {
        EDITOR_INFO("openVisualStudio({0})", filePath);
        std::string command = "start devenv " + filePath;
        system(command.c_str());
    }

    void openPhotoshopTask(const std::string &filePath) {
        EDITOR_INFO("openPhotoshopTask({0})", filePath);
        thread::VoidTask<const std::string&> task = {
                "Photoshop_Task",
                "Photoshop_Thread",
                openPhotoshop
        };
        task.run(filePath);
    }

    void openBlenderTask(const std::string &filePath) {
        EDITOR_INFO("openBlenderTask({0})", filePath);
        thread::VoidTask<const std::string&> task = {
                "Blender_Task",
                "Blender_Thread",
                openBlender
        };
        task.run(filePath);
    }

    void openZBrushTask(const std::string &filePath) {
        EDITOR_INFO("openZBrushTask({0})", filePath);
        thread::VoidTask<const std::string&> task = {
                "ZBrush_Task",
                "ZBrush_Thread",
                openZBrush
        };
        task.run(filePath);
    }

    void msBuildTask(const std::string &slnPath) {
        EDITOR_INFO("msBuildTask({0})", slnPath);
        thread::VoidTask<const std::string&> task = {
                "MSBuild_Task",
                "MSBuild_Thread",
                msBuild
        };
        task.run(slnPath);
    }

    void cmakeTask(const std::string &cmakePath) {
        EDITOR_INFO("cmakeTask({0})", cmakePath);
        thread::VoidTask<const std::string&> task = {
                "CMake_Task",
                "CMake_Thread",
                cmake
        };
        task.run(cmakePath);
    }

    void openPhotoshop(const std::string &filePath) {
        EDITOR_INFO("openPhotoshop({0})", filePath);
        std::string cmd = "start photoshop " + filePath;
        system(cmd.c_str());
    }

    void openBlender(const std::string &filePath) {
        EDITOR_INFO("openBlender({0})", filePath);
        std::string cmd = "blender " + filePath;
        system(cmd.c_str());
    }

    void openZBrush(const std::string &filePath) {
        EDITOR_INFO("openZBrush({0})", filePath);
        std::string cmd = "start zbrush " + filePath;
        system(cmd.c_str());
    }

    void msBuild(const std::string &slnPath) {
        EDITOR_INFO("msBuild({0})", slnPath);
        std::string cmd = "msbuild " + slnPath;
        system(cmd.c_str());
    }

    void cmake(const std::string &cmakePath) {
        EDITOR_INFO("cmake({0})", cmakePath);
        std::string cmd = "cmake " + cmakePath;
        system(cmd.c_str());
    }

    void cmakeD(const std::string &cmakePath, const std::vector<const char *>& definitions) {
        EDITOR_INFO("cmakeTask({0})", cmakePath);
        std::stringstream ss;
        ss << "cmake ";
        for (const char* d : definitions) {
            ss << "-D" << d << " ";
        }
        ss << cmakePath;
        system(ss.str().c_str());
    }

}