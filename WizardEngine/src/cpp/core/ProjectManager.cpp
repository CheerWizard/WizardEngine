//
// Created by mecha on 08.06.2022.
//

#include <core/ProjectManager.h>
#include <core/filesystem.h>
#include <tools/terminal.h>
#include <scripting/ScriptManager.h>
#include <serialization/AssetManager.h>

#include <sstream>
#include <fstream>
#include <regex>

namespace engine::core {

    std::string Project::getFullPath(const char *assetPath) const {
        std::stringstream ss;
        ss << workspacePath << "/" << name << assetPath;
        return ss.str();
    }

    std::string Project::getAssetsPath() const {
        return getFullPath("/assets");
    }

    std::string Project::getAudioPath() const {
        return getFullPath("/assets/audio");
    }

    std::string Project::getMaterialsPath() const {
        return getFullPath("/assets/materials");
    }

    std::string Project::getModelsPath() const {
        return getFullPath("/assets/models");
    }

    std::string Project::getFontsPath() const {
        return getFullPath("/assets/fonts");
    }

    std::string Project::getScenesPath() const {
        return getFullPath("/assets/scenes");
    }

    std::string Project::getScriptsPath() const {
        return getFullPath("/assets/scripts");
    }

    std::string Project::getCMakePath() const {
        return getFullPath("/CMakeLists.txt");
    }

    std::string Project::getCoreShadersPath() const {
        return getFullPath("/core_shaders");
    }

    std::string Project::getFullPath() const {
        std::stringstream ss;
        ss << workspacePath << "/" << name;
        return ss.str();
    }

    std::string Project::getSlnPath() const {
        std::stringstream ss;
        ss << getFullPath() << "/" << name << ".sln";
        return ss.str();
    }

    std::string Project::getExePath(ProjectVersion version) const {
        std::stringstream ss;
        ss << getBuildPath(version) << "/" << name << ".exe";
        return ss.str();
    }

    std::string Project::getBuildPath(ProjectVersion version) const {
        std::stringstream ss;
        ss << getFullPath();
        switch (version) {
            case V_RELEASE:
                ss << "/Release";
                break;
            default:
                ss << "/Debug";
                break;
        }
        return ss.str();
    }

    std::string Project::getZipPath(ProjectVersion version) const {
        std::stringstream ss;
        ss << getBuildPath(version) << "/" << name << ".zip";
        return ss.str();
    }

    vector<Project> ProjectManager::projects;
    Project ProjectManager::currentProject;

    Project ProjectManager::create(const char *projectName, const char *workspacePath) {
        // validate that new project already exists in memory
        for (const auto& project : projects) {
            if (engine::string::equals(project.name.c_str(), projectName)) {
                ENGINE_ERR("ProjectManager: Can't create {0} project. It's already in memory", projectName);
                return project;
            }
        }
        // create project directory using its name
        Project newProject(projectName, workspacePath);
        std::stringstream projectPath;
        projectPath << workspacePath << "/" << projectName;
        if (!engine::filesystem::exists(workspacePath)) {
            engine::filesystem::newDirectory(workspacePath);
        }
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
        ENGINE_INFO("Creating empty asset space for {0} project", newProject.name);
        engine::filesystem::newDirectory(newProject.getAssetsPath());
        engine::filesystem::newDirectory(newProject.getAudioPath());
        engine::filesystem::newDirectory(newProject.getFontsPath());
        engine::filesystem::newDirectory(newProject.getMaterialsPath());
        engine::filesystem::newDirectory(newProject.getModelsPath());
        engine::filesystem::newDirectory(newProject.getScenesPath());
        engine::filesystem::newDirectory(newProject.getScriptsPath());
        // save in memory
        projects.emplace_back(newProject);
        // auto build
        build(newProject, V_DEBUG);
        build(newProject, V_RELEASE);
        buildScripts(newProject, V_DEBUG);
        buildScripts(newProject, V_RELEASE);
        return newProject;
    }

    const Project &ProjectManager::getCurrentProject() {
        return currentProject;
    }

    void ProjectManager::cmake(const Project &project, ProjectVersion projectVersion) {
        // run cmake to generate solution file for project
        vector<const char*> versionDefinitions;
        switch (projectVersion) {
            case V_RELEASE:
                versionDefinitions.emplace_back("DEBUG=0");
                break;
            case V_DEBUG:
                versionDefinitions.emplace_back("DEBUG=1");
                break;
            case V_VISUAL:
                versionDefinitions.emplace_back("DEBUG=1");
                versionDefinitions.emplace_back("VISUAL=1");
                break;
            case V_PROFILING:
                versionDefinitions.emplace_back("DEBUG=1");
                versionDefinitions.emplace_back("PROFILING=1");
                break;
        }
        engine::terminal::cmakeD(project.getCMakePath(), versionDefinitions);
    }

    void ProjectManager::cmake(const char *projectName, ProjectVersion projectVersion) {
        for (const auto& project : projects) {
            if (engine::string::equals(project.name.c_str(), projectName)) {
                cmake(project, projectVersion);
                break;
            }
        }
    }

    void ProjectManager::cmakeScripts(const Project &project, ProjectVersion projectVersion) {
        // run cmake to generate solution file for project
        vector<const char*> versionDefinitions;
        switch (projectVersion) {
            case V_RELEASE:
                versionDefinitions.emplace_back("DEBUG=0");
                break;
            case V_DEBUG:
                versionDefinitions.emplace_back("DEBUG=1");
                break;
            case V_VISUAL:
                versionDefinitions.emplace_back("DEBUG=1");
                versionDefinitions.emplace_back("VISUAL=1");
                break;
            case V_PROFILING:
                versionDefinitions.emplace_back("DEBUG=1");
                versionDefinitions.emplace_back("PROFILING=1");
                break;
        }
        std::stringstream ss;
        ss << project.getFullPath() << "/ScriptEngine/CMakeLists.txt";
        engine::terminal::cmakeD(ss.str(), versionDefinitions);
    }

    void ProjectManager::cmakeScripts(const char *projectName, ProjectVersion projectVersion) {
        for (const auto& project : projects) {
            if (engine::string::equals(project.name.c_str(), projectName)) {
                cmakeScripts(project, projectVersion);
                break;
            }
        }
    }

    void ProjectManager::build(const char *projectName, ProjectVersion projectVersion) {
        for (const auto& project : projects) {
            if (engine::string::equals(project.name.c_str(), projectName)) {
                build(project, projectVersion);
                break;
            }
        }
    }

    void ProjectManager::build(const Project &project, ProjectVersion projectVersion) {
        cmake(project, projectVersion);
        terminal::MSBuildVersion version = terminal::MS_DEBUG;
        if (projectVersion == V_RELEASE) {
            version = terminal::MS_RELEASE;
        }
        engine::terminal::msbuild({ project.getSlnPath().c_str(), version });
        postBuild(project, projectVersion);
    }

    void ProjectManager::buildScripts(const char *projectName, ProjectVersion projectVersion) {
        for (const auto& project : projects) {
            if (engine::string::equals(project.name.c_str(), projectName)) {
                buildScripts(project, projectVersion);
                break;
            }
        }
    }

    void ProjectManager::buildScripts(const Project &project, ProjectVersion projectVersion) {
        cmakeScripts(project, projectVersion);
        terminal::MSBuildVersion version = terminal::MS_DEBUG;
        if (projectVersion == V_RELEASE) {
            version = terminal::MS_RELEASE;
        }
        std::stringstream ss;
        ss << project.getFullPath() << "/ScriptEngine/ScriptEngine.sln";
        engine::terminal::msbuild({ ss.str().c_str(), version });
        postBuildScripts(project, projectVersion);
    }

    void ProjectManager::postBuild(const Project &project, ProjectVersion projectVersion) {
        if (!filesystem::copyRecursive((project.getFullPath() + "/WizardEngine.dll").c_str(), project.getBuildPath(projectVersion).c_str())) {
            ENGINE_ERR("Unable to copy WizardEngine.dll into '{0}'", project.getBuildPath(projectVersion));
        }
        if (!filesystem::copyRecursive((project.getFullPath() + "/assimp-vc143-mtd.dll").c_str(), project.getBuildPath(projectVersion).c_str())) {
            ENGINE_ERR("Unable to copy assimp-vc143-mtd.dll into '{0}'", project.getBuildPath(projectVersion));
        }
        if (!filesystem::copyRecursive((project.getFullPath() + "/OpenAL32.dll").c_str(), project.getBuildPath(projectVersion).c_str())) {
            ENGINE_ERR("Unable to copy OpenAL32.dll into '{0}'", project.getBuildPath(projectVersion));
        }
        if (!filesystem::copyRecursive((project.getFullPath() + "/yaml-cppd.dll").c_str(), project.getBuildPath(projectVersion).c_str())) {
            ENGINE_ERR("Unable to copy yaml-cppd.dll into '{0}'", project.getBuildPath(projectVersion));
        }
        // copy assets used for project
        if (!filesystem::copyRecursive(project.getAssetsPath().c_str(), (project.getBuildPath(projectVersion) + "/assets").c_str())) {
            ENGINE_ERR("Unable to copy assets directory into '{0}'", project.getBuildPath(projectVersion));
            ENGINE_INFO("Creating empty asset space for {0} project", project.name);
            engine::filesystem::newDirectory(project.getAssetsPath());
            engine::filesystem::newDirectory(project.getAudioPath());
            engine::filesystem::newDirectory(project.getFontsPath());
            engine::filesystem::newDirectory(project.getMaterialsPath());
            engine::filesystem::newDirectory(project.getModelsPath());
            engine::filesystem::newDirectory(project.getScenesPath());
            engine::filesystem::newDirectory(project.getScriptsPath());
        }
        // remove scripts assets from build path
        std::string scriptsBinaryPath = project.getBuildPath(projectVersion) + "/assets/scripts";
        engine::filesystem::removeAll(scriptsBinaryPath);
        engine::filesystem::remove(scriptsBinaryPath);
        // copy core engine shaders used for project
        if (!filesystem::copyRecursive(project.getCoreShadersPath().c_str(), (project.getBuildPath(projectVersion) + "/core_shaders").c_str())) {
            ENGINE_ERR("Unable to copy core_shaders directory into '{0}'", project.getBuildPath(projectVersion));
        }
    }

    void ProjectManager::postBuildScripts(const Project &project, ProjectVersion projectVersion) {
        if (!filesystem::copyRecursive(getScriptEngineBuildPath(project, projectVersion).c_str(), project.getBuildPath(projectVersion).c_str())) {
            ENGINE_ERR("Unable to copy ScriptEngine.dll into '{0}'", project.getBuildPath(projectVersion));
        }
        loadScripts(project, projectVersion);
    }

    void ProjectManager::run(const char *projectName, ProjectVersion projectVersion) {
        for (const auto& project : projects) {
            if (engine::string::equals(project.name.c_str(), projectName)) {
                run(project, projectVersion);
                break;
            }
        }
    }

    void ProjectManager::run(const Project& project, ProjectVersion projectVersion) {
        // todo crashes inside thread runnable lambda
//        thread::VoidTask<const Project&> task = {
//                project.name,
//                project.name,
//                runImpl
//        };
//        task.run(project);
        runImpl(project, projectVersion);
    }

    void ProjectManager::runImpl(const Project& project, ProjectVersion projectVersion) {
        terminal::exe(project.getExePath(projectVersion));
    }

    void ProjectManager::saveProjects() {
        if (projects.empty()) {
            ENGINE_WARN("ProjectManager::saveProjects: no projects available to save");
            return;
        }

        std::ofstream file("projects.cfg");

        file << projects[0].workspacePath << "\n";
        for (u32 i = 0 ; i < projects.size() ; i++) {
            const auto& project = projects[i];
            ENGINE_INFO("ProjectManager: saving project name = {0}, workspace = {1}",
                        project.name, project.workspacePath);
            file << project.name;
            if (i == projects.size() - 1) break;
            file << "\n";
        }

        file.close();
    }

    void ProjectManager::loadProjects() {
        std::ifstream file("projects.cfg");
        if (!file.is_open()) {
            ENGINE_ERR("ProjectManager: Unable to load projects from {0}", "projects.cfg");
            return;
        }

        std::string workspacePath;
        std::getline(file, workspacePath);

        std::string projectName;
        projects.clear();
        while (std::getline(file, projectName)) {
            projects.emplace_back(projectName, workspacePath);
        }

        for (const auto& project : projects) {
            ENGINE_INFO("ProjectManager: project loaded name = {0}, workspace = {1}",
                        project.name, project.workspacePath);
        }

        file.close();
    }

    void ProjectManager::setCurrentProject(const char* projectName) {
        for (const Project& project : projects) {
            if (engine::string::equals(projectName, project.name.c_str())) {
                currentProject = project;
                return;
            }
        }
    }

    void ProjectManager::setCurrentProject(const Project& project) {
        currentProject = project;
    }

    const vector<Project> &ProjectManager::getAll() {
        return projects;
    }

    bool ProjectManager::destroy(const char *projectName) {
        for (u32 i = 0 ; i < projects.size() ; i++) {
            if (engine::string::equals(projectName, projects.at(i).name.c_str())) {
                engine::filesystem::removeAll(projects.at(i).getFullPath());
                engine::filesystem::remove(projects.at(i).getFullPath());
                projects.erase(projects.begin() + i);
                return true;
            }
        }
        return false;
    }

    void ProjectManager::open(const char *projectName) {
        for (const auto& project : projects) {
            if (engine::string::equals(projectName, project.name.c_str())) {
                engine::terminal::openVisualStudio(project.getSlnPath());
            }
        }
    }

    void ProjectManager::openScripts(const char *projectName) {
        for (const auto& project : projects) {
            if (engine::string::equals(projectName, project.name.c_str())) {
                openScripts(project);
            }
        }
    }

    void ProjectManager::openScripts(const Project& project) {
        std::stringstream ss;
        ss << project.getFullPath() << "/ScriptEngine/ScriptEngine.sln";
        engine::terminal::openVisualStudio(ss.str());
    }

    void ProjectManager::publish(const char *projectName, ProjectVersion projectVersion) {
        for (const auto& project : projects) {
            if (engine::string::equals(projectName, project.name.c_str())) {
                publish(project, projectVersion);
            }
        }
    }

    void ProjectManager::publish(const Project &project, ProjectVersion projectVersion) {
        std::string zipPath = project.name + ".zip";
        std::string zipBuildPath = project.getZipPath(projectVersion);
        std::string buildPath = project.getBuildPath(projectVersion);
        if (engine::filesystem::exists(zipBuildPath)) {
            engine::filesystem::remove(zipBuildPath);
        }
        engine::filesystem::zip(zipPath.c_str(), buildPath.c_str());
        engine::filesystem::copyRecursive(zipPath.c_str(), buildPath.c_str());
    }

    std::string ProjectManager::getScriptEngineBuildPath(const Project &project, ProjectVersion projectVersion) {
        std::stringstream ss;
        switch (projectVersion) {
            case V_RELEASE:
                ss << project.getFullPath() << "/ScriptEngine" << "/Release/" << "ScriptEngine.dll";
                break;
            default:
                ss << project.getFullPath() << "/ScriptEngine" << "/Debug/" << "ScriptEngine.dll";
                break;
        }
        return ss.str();
    }

    void ProjectManager::loadScripts(const Project& project, ProjectVersion projectVersion) {
        scripting::ScriptManager::free();
        scripting::ScriptManager::init(getScriptEngineBuildPath(project, projectVersion));
    }

    void ProjectManager::renameScript(const std::string &oldPath, const std::string &newName) {
        std::string script = engine::filesystem::read(oldPath);
        std::string oldName = engine::filesystem::getFileName(oldPath);
        script = std::regex_replace(script, std::regex("class " + oldName), "class " + newName);
        script = std::regex_replace(script,std::regex("(" + oldName + ")"),newName);
        script = std::regex_replace(script, std::regex("\n"), "");
        engine::filesystem::write(oldPath, script.c_str());
        engine::filesystem::rename(oldPath, newName + ".cpp");
    }

    void ProjectManager::newScript(const std::string &filePath, const std::string &name) {
        std::string scriptTemplate = engine::filesystem::read(currentProject.getFullPath() + "/ScriptEngine/Template.cpp");
        scriptTemplate = std::regex_replace(scriptTemplate, std::regex("Template"), name);
        scriptTemplate = std::regex_replace(scriptTemplate, std::regex("\n"), "");
        engine::filesystem::write(filePath, scriptTemplate);
    }

    void Project::loadScenes() {
        io::LocalAssetManager::assetsPath = getAssetsPath();
        scenes = io::LocalAssetManager::loadAll();
    }

    void Project::saveScenes() const {
        io::LocalAssetManager::assetsPath = getAssetsPath();
        io::LocalAssetManager::saveAll(scenes);
    }

}