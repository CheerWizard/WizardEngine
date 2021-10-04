cmake_minimum_required(VERSION 3.20)

project(FairyLab)

# creating .exe from sources.
file(GLOB_RECURSE FAIRY_SRC src/*.cpp)
add_executable(FairyLab ${FAIRY_SRC})
replace_dirs(FairyLab/assets FairyLab/assets)

target_link_libraries(FairyLab WizardEngine)