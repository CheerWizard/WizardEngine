//
// Created by mecha on 30.01.2022.
//

#pragma once

#include <string_view>

#define PLATFORM_CPP_PATH "../WizardEngine/src/cpp/platform"
#define ENGINE_PROJECT_PATH "../WizardEngine/WizardEngine.sln"
#define ENGINE_CMAKE_PATH "../WizardEngine/CMakeLists.txt"

namespace engine {

    enum OperatingSystem {
        WINDOWS,
        LINUX,
        MACOS
    };

    enum GraphicsVendor {
        OPENGL
    };

    struct Platform {
        OperatingSystem os;
        std::string osPath;
        GraphicsVendor gv;
        std::string gvPath;

        Platform(const OperatingSystem &os, const GraphicsVendor &gv) : os(os), gv(gv) {
            init();
        }

        void init() {
            switch (os) {
                case WINDOWS:
                    osPath = "../WizardEngine/windows";
                    break;
                case LINUX:
                    osPath = "../WizardEngine/linux";
                    break;
                case MACOS:
                    osPath = "../WizardEngine/macos";
                    break;
            }
            switch (gv) {
                case OPENGL:
                    gvPath = "../WizardEngine/opengl";
                    break;
            }
        }
    };

    struct ProjectProps {
        std::string name;
        bool isRunning;
    };

    class ProjectBuilder final {

    public:
        static void buildTask(const Platform& platform);
        static void buildPlatform(const Platform& platform);
        static void buildTask(const ProjectProps& projectProps);
        static void buildProject(const ProjectProps& projectProps);
        static void runProject(const ProjectProps& projectProps);
        static void runProjectTask(const ProjectProps& projectProps);

    private:
        static void build(const std::string& cmakePath, const std::string& projectPath);
        static void run(const ProjectProps& projectProps, const std::string& buildPath);

    };

}