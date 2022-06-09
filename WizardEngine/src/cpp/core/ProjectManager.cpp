//
// Created by mecha on 08.06.2022.
//

#include <core/ProjectManager.h>
#include <core/filesystem.h>
#include <tools/terminal.h>

#include <sstream>

namespace engine::core {

    std::string Project::getFullPath(const char *assetPath) const {
        std::stringstream ss;
        ss << workspacePath << name << assetPath;
        return ss.str();
    }

    std::string Project::getAssetsPath() {
        return getFullPath("/assets");
    }

    std::string Project::getAudioPath() {
        return getFullPath("/assets/audio");
    }

    std::string Project::getMaterialsPath() {
        return getFullPath("/assets/materials");
    }

    std::string Project::getModelsPath() {
        return getFullPath("/assets/models");
    }

    std::string Project::getFontsPath() {
        return getFullPath("/assets/fonts");
    }

    std::string Project::getScenesPath() {
        return getFullPath("/assets/scenes");
    }

    std::string Project::getScriptsPath() {
        return getFullPath("/assets/scripts");
    }

    std::string Project::getCMakePath() {
        return getFullPath("/CMakeLists.txt");
    }

    vector<Project> ProjectManager::projects;
    Project ProjectManager::currentProject;

    Project ProjectManager::create(const char *projectName, const char *workspacePath) {
        // create project directory using its name
        Project newProject(projectName, workspacePath);
        std::stringstream projectPath;
        projectPath << workspacePath << projectName;
        engine::filesystem::newDirectory(projectPath.str().c_str());
        // create CMakeLists.txt for build/link process
        auto cmakeIncludes = engine::filesystem::read("../WizardEngine/project_template/cmake_includes.txt");
        auto cmakeSrc = engine::filesystem::read("../WizardEngine/project_template/cmake_src.txt");
        std::stringstream cmakeProjectName;
        cmakeProjectName << "\n\n" << "project(" << projectName << ")" << "\n\n";
        std::stringstream cmakeLists;
        cmakeLists << cmakeIncludes << cmakeProjectName.str() << cmakeSrc;
        engine::filesystem::write(projectPath.str() + "/CMakeLists.txt",cmakeLists.str());
        // copy project template into projects path
        engine::filesystem::copyRecursive("../WizardEngine/project_template", projectPath.str().c_str());
        // create asset space
        engine::filesystem::newDirectory(newProject.getAssetsPath());
        engine::filesystem::newDirectory(newProject.getAudioPath());
        engine::filesystem::newDirectory(newProject.getFontsPath());
        engine::filesystem::newDirectory(newProject.getMaterialsPath());
        engine::filesystem::newDirectory(newProject.getModelsPath());
        engine::filesystem::newDirectory(newProject.getScenesPath());
        engine::filesystem::newDirectory(newProject.getScriptsPath());

        // run cmake
        engine::terminal::cmake(newProject.getCMakePath());

        // save in memory
        projects.emplace_back(newProject);
        return newProject;
    }

    const Project &ProjectManager::getCurrentProject() {
        return currentProject;
    }
}