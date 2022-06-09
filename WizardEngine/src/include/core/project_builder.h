//
// Created by mecha on 08.06.2022.
//

#pragma once

#include <core/vector.h>
#include <string>

namespace engine::core {

    struct Project {
        const char* name = nullptr;
        const char* workspacePath = nullptr;

        Project() = default;
        Project(const char* name, const char* workspacePath)
        : name(name), workspacePath(workspacePath) {}

        inline std::string getAssetsPath();
        inline std::string getAudioPath();
        inline std::string getMaterialsPath();
        inline std::string getModelsPath();
        inline std::string getFontsPath();
        inline std::string getScenesPath();
        inline std::string getScriptsPath();

    private:
        std::string getFullPath(const char* assetPath) const;
    };

    class ProjectManager final {

    public:
        static Project create(const char* projectName, const char* workspacePath);
        inline static const Project& getCurrentProject();

    private:
        static vector<Project> projects;
        static Project currentProject;
    };

}
