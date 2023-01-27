cmake_minimum_required(VERSION 3.20)

project(Wizard)

add_subdirectory(WizardEngine)
add_subdirectory(WizardSamples)
#add_subdirectory(WizardStudio)
add_subdirectory(WizardTest)
#add_subdirectory(WizardServer)

replace_dirs(cmake_tools WizardEngine/cmake_tools)
replace_dirs(cmake_tools WizardStudio/cmake_tools)
replace_dirs(cmake_tools WizardServer/cmake_tools)
replace_dirs(cmake_tools WizardTest/cmake_tools)