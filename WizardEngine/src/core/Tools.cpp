//
// Created by mecha on 09.10.2021.
//

#include "Tools.h"

namespace engine {

    void Tools::openVSCode(const std::string &filePath) {
        EDITOR_INFO("openVSCode({0})", filePath);
        VoidTask<const std::string&> task = {
                "VSCode_Task",
                "VSCode_Thread",
                openVSCodeTask
        };
        task.run(filePath);
    }

    void Tools::openVSCodeTask(const std::string &filePath) {
        EDITOR_INFO("openVSCodeTask({0})", filePath);
        std::string command = "code " + filePath;
        system(command.c_str());
    }

    void Tools::openNotepad(const std::string &filePath) {
        EDITOR_INFO("openNotepad({0})", filePath);
        VoidTask<const std::string&> task = {
                "VSCode_Task",
                "VSCode_Thread",
                openVSCodeTask
        };
        task.run(filePath);
    }

    void Tools::openNotepadTask(const std::string &filePath) {
        EDITOR_INFO("openNotepadTask({0})", filePath);
        std::string command = "notepad " + filePath;
        system(command.c_str());
    }

    void Tools::openVisualStudio(const std::string &filePath) {
        EDITOR_INFO("openVisualStudio({0})", filePath);
        VoidTask<const std::string&> task = {
                "VSCode_Task",
                "VSCode_Thread",
                openVSCodeTask
        };
        task.run(filePath);
    }

    void Tools::openVisualStudioTask(const std::string &filePath) {
        EDITOR_INFO("openVisualStudioTask({0})", filePath);
        std::string command = "start devenv " + filePath;
        system(command.c_str());
    }

    void Tools::openPhotoshop(const std::string &filePath) {
        EDITOR_INFO("openPhotoshop({0})", filePath);
        VoidTask<const std::string&> task = {
                "Photoshop_Task",
                "Photoshop_Thread",
                openPhotoshopTask
        };
        task.run(filePath);
    }

    void Tools::openBlender(const std::string &filePath) {
        EDITOR_INFO("openBlender({0})", filePath);
        VoidTask<const std::string&> task = {
                "Blender_Task",
                "Blender_Thread",
                openBlenderTask
        };
        task.run(filePath);
    }

    void Tools::openZBrush(const std::string &filePath) {
        EDITOR_INFO("openZBrush({0})", filePath);
        VoidTask<const std::string&> task = {
                "ZBrush_Task",
                "ZBrush_Thread",
                openZBrushTask
        };
        task.run(filePath);
    }

    void Tools::openPhotoshopTask(const std::string &filePath) {
        EDITOR_INFO("openPhotoshopTask({0})", filePath);
        std::string cmd = "start photoshop " + filePath;
        system(cmd.c_str());
    }

    void Tools::openBlenderTask(const std::string &filePath) {
        EDITOR_INFO("openBlenderTask({0})", filePath);
        std::string cmd = "blender " + filePath;
        system(cmd.c_str());
    }

    void Tools::openZBrushTask(const std::string &filePath) {
        EDITOR_INFO("openZBrushTask({0})", filePath);
        std::string cmd = "start zbrush " + filePath;
        system(cmd.c_str());
    }

}