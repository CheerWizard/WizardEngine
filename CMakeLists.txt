cmake_minimum_required(VERSION 3.20)

project(Wizard)

# Macros to build project for specific platform. WizardEngine project will detect and process platform under the hood.
# We should always set it , before adding subprojects!
if (DEBUG)
    add_definitions(-DDEBUG=1)
endif (DEBUG)

if (WIN32)
    add_definitions(-DWIN32=1)
endif (WIN32)

if (WIN64)
    add_definitions(-DWIN64=1)
endif (WIN64)

# Subprojects from Wizard.
add_subdirectory(WizardEngine)
add_subdirectory(WizardTest)