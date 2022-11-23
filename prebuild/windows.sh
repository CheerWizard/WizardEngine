# replace CMake file for ImGui
cp imgui/CMakeLists.txt ../WizardEngine/vendor/imgui/CMakeLists.txt
# update docking branch of ImGui
cd ../WizardEngine/vendor/imgui
git checkout docking
# build Assimp is required
cd ../WizardEngine/vendor/assimp
cmake CMakeLists.txt
cmake --build .