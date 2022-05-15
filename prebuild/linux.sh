# setup X11 dev tools for GLFW
sudo apt install libx11-dev
sudo apt install libxrandr-dev
sudo apt install libxinerama-dev
sudo apt install libxcursor-dev
sudo apt install libxi-dev
sudo apt install libxext-dev
# replace CMake file for ImGui
cp imgui/CMakeLists.txt ../WizardEngine/vendor/imgui/CMakeLists.txt
# update docking branch of ImGui
cd ../WizardEngine/vendor/imgui
git checkout docking