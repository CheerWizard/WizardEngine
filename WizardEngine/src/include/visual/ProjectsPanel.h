//
// Created by mecha on 04.10.2022.
//

#pragma once

#include <visual/Widgets.h>
#include <core/ProjectManager.h>

namespace engine::visual {

    using namespace math;

    enum ProjectOptionTab {
        TAB_NEW_PROJECT,
        TAB_EXISTING_PROJECTS
    };

    class ENGINE_API ProjectsPanel final {

    public:
        static void init();
        static void setTheme();
        static void draw();
        static void destroy();

    private:
        static ProjectOptionTab tab;
        static std::string newProjectName;
        static std::string newWorkspace;
        static Project selectedProject;
        static bool isProjectSelected;
    };

}
