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

if (GLFW)
    add_definitions(-DGLFW=1)
endif(GLFW)

if (GL)
    add_definitions(-DGL=1)
endif(GL)

if (VK)
    add_definitions(-DVK=1)
endif(VK)

# copying target dir to destination dir. If destination does not exist, create it.
macro(copy_dir dir1 dir2)
    set(srcDir ${CMAKE_SOURCE_DIR}/${dir1})
    set(destDir ${CMAKE_BINARY_DIR}/${dir2})

    message(STATUS "Creating directory ${destDir}")
    make_directory(${destDir})

    message(STATUS "Search files in directory ${srcDir}")
    file(GLOB_RECURSE templateFiles RELATIVE ${srcDir} ${srcDir}/*)

    message(STATUS "Copying files from directory ${srcDir}")
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            message(STATUS "Target file path ${srcTemplatePath}")
            message(STATUS "Destination file path ${destDir}/${templateFile}")
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    COPYONLY)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)

endmacro(copy_dir)

# delete build directory
macro(delete_build_dir dir)
    set(buildDir ${CMAKE_BINARY_DIR}/${dir})
    message(STATUS "Deleting build directory ${buildDir}")
    file(REMOVE_RECURSE ${buildDir})
endmacro(delete_build_dir)

# delete destination dir and copy target dir into destination dir.
macro(replace_dirs targetDir destDir)
    delete_build_dir(${destDir})
    copy_dir(${targetDir} ${destDir})
endmacro(replace_dirs)

# Subprojects from Wizard.
add_subdirectory(WizardEngine)
add_subdirectory(FairyLab)