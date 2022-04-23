//
// Created by mecha on 30.01.2022.
//

#include <build/ProjectBuilder.h>
#include <io/Logger.h>
#include <core/filesystem.h>
#include <build/Build.h>
#include <tools/terminal.h>
#include <sstream>

namespace engine::build {

    void ProjectBuilder::buildTask(const Platform &platform) {
        thread::VoidTask<const Platform&> task = {
                "PlatformBuild_Task",
                "PlatformBuild_Thread",
                buildPlatform
        };
        task.run(platform);
    }

    void ProjectBuilder::buildPlatform(const Platform& platform) {
        ENGINE_INFO("build({0}, {1})", platform.osPath, platform.gvPath);
        // replace OS cpp packages into platform
        std::string platformCppPath(PLATFORM_CPP_PATH);
        filesystem::replace(platform.osPath + "/core", platformCppPath + "/core");
        filesystem::replace(platform.osPath + "/tools", platformCppPath + "/tools");
        // replace graphics vendor packages into platform
        filesystem::replace(platform.gvPath + "/graphics", platformCppPath + "/graphics");
        // build WizardEngine project
        build(ENGINE_CMAKE_PATH, ENGINE_PROJECT_PATH);
    }

    void ProjectBuilder::buildTask(const ProjectProps& projectProps) {
        thread::VoidTask<const ProjectProps&> task = {
                "ProjectBuild_Task",
                "ProjectBuild_Thread",
                buildProject
        };
        task.run(projectProps);
    }

    void ProjectBuilder::buildProject(const ProjectProps &projectProps) {
        ENGINE_INFO("build({0})", projectProps.name);
        if (projectProps.isRunning) {
            build(projectProps.name + ".sln", "CMakeLists.txt");
        } else {
            std::stringstream projectPath;
            std::stringstream cmakePath;
            projectPath << "../" << projectProps.name << "/" << projectProps.name << ".sln";
            cmakePath << "../" << projectProps.name << "/" << "CMakeLists.txt";
            build(cmakePath.str(), projectPath.str());
        }
    }

    void ProjectBuilder::build(const std::string &cmakePath, const std::string& projectPath) {
        if (!filesystem::exists(projectPath)) {
            terminal::cmakeD(cmakePath, { "DEBUG" });
        }
        terminal::msBuild(projectPath);
    }

    void ProjectBuilder::runProject(const ProjectProps& projectProps) {
        ENGINE_INFO("runProject({0})", projectProps.name);
        if (projectProps.isRunning) {
            run(projectProps, "Debug/" + projectProps.name + ".exe");
        } else {
            std::stringstream buildPath;
            buildPath << "../" << projectProps.name << "/Debug/" << projectProps.name << ".exe";
            run(projectProps, buildPath.str());
        }
    }

    void ProjectBuilder::runProjectTask(const ProjectProps &projectProps) {
        thread::VoidTask<const ProjectProps&> task = {
                "RunProject_Task",
                "RunProject_Thread",
                runProject
        };
        task.run(projectProps);
    }

    void ProjectBuilder::run(const ProjectProps &projectProps, const std::string& buildPath) {
        if (!filesystem::exists(buildPath)) {
            buildProject(projectProps);
        }
        Executable::run(buildPath);
    }

}