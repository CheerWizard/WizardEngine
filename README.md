![WizardGE_post](https://user-images.githubusercontent.com/37471793/159553487-0ee2a6bb-04d0-4c68-b0bb-bab22804a3ca.png) 
# Wizard Engine

## Manual build instructions

### Install CMake
Project can be build for **Windows** platform.
For **Linux** and **macOS** it's **NOT** well-developed yet and some features and systems may not be stable.  
Project uses **CMake** to generate solution file for **Visual Studio** environment. 
You can also use **CLion IDE** environment for generating and building this project.
You have to make sure that you have a working cmake-installation on your system. 
You can download it here https://cmake.org/

### Get the source
Make sure you have a working git-installation. Open a command prompt and clone the WizardEngine project via:
```bash
git clone --recursive https://github.com/CheerWizard/WizardEngine
```
**IMPORTANT**: Make sure you are cloning repo recursively with "--recursive" keyword. 
Otherwise, it won't fetch all dependencies and as a result - build process will fail!

### Build from source:
```bash
cd WizardEngine
cmake CMakeLists.txt 
cmake --build .
```

### Build instructions for Windows with Visual Studio

First you have to install **Visual Studio** on your Windows system. You can get the **Community Version** 
for free here: https://visualstudio.microsoft.com/de/downloads/

To generate the build environment for your IDE open a command prompt, navigate to your repo and type:
```bash
cmake CMakeLists.txt
```
This will generate the project files for the visual studio. 
All dependencies used to build **WizardEngine** shall be part of the repo.

### Build instructions for Windows with CLion IDEA
Build project with **CLion IDEA** is much easier. If you already cloned Git repository, then just follow these steps:
1. Open **WizardEngine** package with **CLion IDEA** 
2. Wait until CMake commands will be executed
3. Select build configuration :
- **WizardEngine|Debug** or **WizardEngine|Release** for building game engine library .lib
- **WizardStudio|Debug** or **WizardStudio|Release** for building game development editor .exe program
- **WizardTest|Debug** or **WizardTest|Release** for building game test .exe program
4. Press on build hammer (Ctrl+F9) to build configuration you want to 

## WizardEngine website and support
Project website link : https://cheerwizard.github.io/WizardEngine/

## Resources and references
Thanks to all these resources for giving a lot of knowledge for creating this project.
Below will be the list of channels and websites where I am getting my knowledge.
### YouTube channels
- https://www.youtube.com/c/TheChernoProject
- https://www.youtube.com/c/GameEngineSeries
- https://www.youtube.com/user/thebennybox
- https://www.youtube.com/user/CppCon
- https://www.youtube.com/c/VictorGordan
### Websites and Wiki
- https://learnopengl.com/
- https://vulkan-tutorial.com/
- https://www.gamedev.net/
- https://www.khronos.org/opengl/wiki/