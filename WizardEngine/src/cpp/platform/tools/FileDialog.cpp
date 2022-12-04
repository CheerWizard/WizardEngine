
//
// Created by mecha on 08.10.2021.
//

#include <platform/tools/FileDialog.h>

#include <Windows.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32 // avoids "include" conflicts
#include <GLFW/glfw3native.h>

namespace engine::tools {

    std::string FileDialog::getImportPath(const char *filter, const char* initialDir) {
        ENGINE_INFO("FileDialog::getImportPath(): filter={0} , initialDir={1}", filter, initialDir);

        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };

        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*) nativeWindow);
        ofn.lpstrFile = szFile;
        ofn.lpstrInitialDir = initialDir;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileName(&ofn) == TRUE) {
            return ofn.lpstrFile;
        }

        return "";
    }

    std::string FileDialog::getExportPath(const char *filter, const char* initialDir) {
        ENGINE_INFO("FileDialog::getExportPath(): filter={0} , initialDir={1}", filter, initialDir);

        OPENFILENAMEA ofn;
        CHAR szFile[260] = { 0 };

        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*) nativeWindow);
        ofn.lpstrFile = szFile;
        ofn.lpstrInitialDir = initialDir;
        ofn.nMaxFile = sizeof(szFile);
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