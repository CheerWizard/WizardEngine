//
// Created by mecha on 09.10.2021.
//

#include "FileEditor.h"

namespace engine {

    Task<void(const std::string&), const std::string&> FileEditor::notepadTask = {
            "Notepad_Task",
            "Notepad_Thread",
            openNotepadTask
    };

    Task<void(const std::string&), const std::string&> FileEditor::vsCodeTask = {
            "VSCode_Task",
            "VSCode_Thread",
            openVSCodeTask
    };

    Task<void(const std::string&), const std::string&> FileEditor::visualStudioTask = {
            "VisualStudio_Task",
            "VisualStudio_Thread",
            openVisualStudioTask
    };

    void FileEditor::openVSCode(const std::string &filePath) {
        EDITOR_INFO("openVSCode({0})", filePath);
        vsCodeTask.run(filePath);
    }

    void FileEditor::openVSCodeTask(const std::string &filePath) {
        EDITOR_INFO("openVSCodeTask({0})", filePath);
        std::string command = "code " + filePath;
        system(command.c_str());
    }

    void FileEditor::openNotepad(const std::string &filePath) {
        EDITOR_INFO("openNotepad({0})", filePath);
        notepadTask.run(filePath);
    }

    void FileEditor::openNotepadTask(const std::string &filePath) {
        EDITOR_INFO("openNotepadTask({0})", filePath);
        std::string command = "notepad " + filePath;
        system(command.c_str());
    }

    void FileEditor::openVisualStudio(const std::string &filePath) {
        EDITOR_INFO("openVisualStudio({0})", filePath);
        visualStudioTask.run(filePath);
    }

    void FileEditor::openVisualStudioTask(const std::string &filePath) {
        EDITOR_INFO("openVisualStudioTask({0})", filePath);
        std::string command = "start devenv " + filePath;
        system(command.c_str());
    }

}