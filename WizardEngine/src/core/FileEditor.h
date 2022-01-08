//
// Created by mecha on 09.10.2021.
//

#pragma once

#include "Task.h"

namespace engine {

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
        static Task<void(const std::string&), const std::string&> notepadTask;
        static Task<void(const std::string&), const std::string&> vsCodeTask;
        static Task<void(const std::string&), const std::string&> visualStudioTask;
    };

}