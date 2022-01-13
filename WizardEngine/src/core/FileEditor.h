//
// Created by mecha on 09.10.2021.
//

#pragma once

#include "Task.h"

namespace engine {

    typedef Task<void(const std::string&), const std::string&> FileEditorTask;

    class FileEditor final {

    public:
        static void openNotepad(const std::string &filePath);
        static void openVSCode(const std::string &filePath);
        static void openVisualStudio(const std::string &filePath);

    private:
        static void openNotepadTask(const std::string &filePath);
        static void openVSCodeTask(const std::string &filePath);
        static void openVisualStudioTask(const std::string &filePath);

    private:
        static FileEditorTask notepadTask;
        static FileEditorTask vsCodeTask;
        static FileEditorTask visualStudioTask;
    };

}