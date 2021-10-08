//
// Created by mecha on 08.10.2021.
//

#include "WindowsFileDialog.h"

#include <Windows.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace engine {

    std::string WindowsFileDialog::getImportPath(const char *filter) {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };

        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*) nativeWindow);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);

        if (GetCurrentDirectoryA(256, currentDir)) {
            ofn.lpstrInitialDir = currentDir;
        }

        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileName(&ofn) == TRUE) {
            return ofn.lpstrFile;
        }

        return "";
    }

    std::string WindowsFileDialog::getExportPath(const char *filter) {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };

        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*) nativeWindow);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);

        if (GetCurrentDirectoryA(256, currentDir)) {
            ofn.lpstrInitialDir = currentDir;
        }

        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        // Sets the default extension by extracting it from the filter
        ofn.lpstrDefExt = strchr(filter, '\0') + 1;

        if (GetSaveFileName(&ofn) == TRUE) {
            return ofn.lpstrFile;
        }

        return "";
    }

}