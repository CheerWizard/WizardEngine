# setup X11 dev tools for GLFW
sudo apt install libx11-dev
sudo apt install libxrandr-dev
sudo apt install libxinerama-dev
sudo apt install libxcursor-dev
sudo apt install libxi-dev
sudo apt install libxext-dev
# replace CMake file for ImGui
cd ../WizardEngine/vendor/imgui
git checkout docking
cd ..
git add imgui
cp ../../prebuild/imgui/CMakeLists.txt imgui/CMakeLists.txt