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

    private:
        ProjectsPanel();
        ~ProjectsPanel();

    public:
        static ProjectsPanel& get();
        void draw();

    public:
        bool enableDraw = false;

    private:
        ProjectOptionTab tab = TAB_NEW_PROJECT;
        std::string newProjectName = "Untitled";
        std::string newWorkspace = "projects";
        Project selectedProject;
        bool isProjectSelected = false;
        u32 backgroundImage = 0;
    };

}
