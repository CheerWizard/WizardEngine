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
        inline std::string getCMakePath();
        [[nodiscard]] std::string getFullPath() const;
        [[nodiscard]] inline std::string getSlnPath() const;
        [[nodiscard]] inline std::string getExePath() const;

    private:
        std::string getFullPath(const char* assetPath) const;
    };

    class ProjectManager final {

    public:
        static Project create(const char* projectName, const char* workspacePath);
        inline static const Project& getCurrentProject();
        static void build(const char* projectName);
        static void build(const Project& project);
        static void run(const char* projectName);
        static void run(const Project& project);

    private:
        static vector<Project> projects;
        static Project currentProject;
    };

}
