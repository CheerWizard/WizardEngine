# replace CMake file for ImGui
cp imgui/CMakeLists.txt ../WizardEngine/vendor/imgui/CMakeLists.txt
# update docking branch of ImGui
cd ../WizardEngine/vendor/imgui
git checkout docking