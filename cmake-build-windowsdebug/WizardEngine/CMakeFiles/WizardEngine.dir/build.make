# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\mecha\CLionProjects\Wizard

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

# Include any dependencies generated for this target.
include WizardEngine\CMakeFiles\WizardEngine.dir\depend.make
# Include the progress variables for this target.
include WizardEngine\CMakeFiles\WizardEngine.dir\progress.make

# Include the compile flags for this target's objects.
include WizardEngine\CMakeFiles\WizardEngine.dir\flags.make

WizardEngine\CMakeFiles\WizardEngine.dir\src\Launcher.cpp.obj: WizardEngine\CMakeFiles\WizardEngine.dir\flags.make
WizardEngine\CMakeFiles\WizardEngine.dir\src\Launcher.cpp.obj: ..\WizardEngine\src\Launcher.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object WizardEngine/CMakeFiles/WizardEngine.dir/src/Launcher.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\WizardEngine.dir\src\Launcher.cpp.obj /FdCMakeFiles\WizardEngine.dir\WizardEngine.pdb /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\Launcher.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\Launcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WizardEngine.dir/src/Launcher.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\WizardEngine.dir\src\Launcher.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\Launcher.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\Launcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WizardEngine.dir/src/Launcher.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\WizardEngine.dir\src\Launcher.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\Launcher.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Application.cpp.obj: WizardEngine\CMakeFiles\WizardEngine.dir\flags.make
WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Application.cpp.obj: ..\WizardEngine\src\core\Application.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object WizardEngine/CMakeFiles/WizardEngine.dir/src/core/Application.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\WizardEngine.dir\src\core\Application.cpp.obj /FdCMakeFiles\WizardEngine.dir\WizardEngine.pdb /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\core\Application.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Application.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WizardEngine.dir/src/core/Application.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\WizardEngine.dir\src\core\Application.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\core\Application.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Application.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WizardEngine.dir/src/core/Application.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\WizardEngine.dir\src\core\Application.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\core\Application.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Layer.cpp.obj: WizardEngine\CMakeFiles\WizardEngine.dir\flags.make
WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Layer.cpp.obj: ..\WizardEngine\src\core\Layer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object WizardEngine/CMakeFiles/WizardEngine.dir/src/core/Layer.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\WizardEngine.dir\src\core\Layer.cpp.obj /FdCMakeFiles\WizardEngine.dir\WizardEngine.pdb /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\core\Layer.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Layer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WizardEngine.dir/src/core/Layer.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\WizardEngine.dir\src\core\Layer.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\core\Layer.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Layer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WizardEngine.dir/src/core/Layer.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\WizardEngine.dir\src\core\Layer.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\core\Layer.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\core\LayerStack.cpp.obj: WizardEngine\CMakeFiles\WizardEngine.dir\flags.make
WizardEngine\CMakeFiles\WizardEngine.dir\src\core\LayerStack.cpp.obj: ..\WizardEngine\src\core\LayerStack.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object WizardEngine/CMakeFiles/WizardEngine.dir/src/core/LayerStack.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\WizardEngine.dir\src\core\LayerStack.cpp.obj /FdCMakeFiles\WizardEngine.dir\WizardEngine.pdb /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\core\LayerStack.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\core\LayerStack.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WizardEngine.dir/src/core/LayerStack.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\WizardEngine.dir\src\core\LayerStack.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\core\LayerStack.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\core\LayerStack.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WizardEngine.dir/src/core/LayerStack.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\WizardEngine.dir\src\core\LayerStack.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\core\LayerStack.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Logger.cpp.obj: WizardEngine\CMakeFiles\WizardEngine.dir\flags.make
WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Logger.cpp.obj: ..\WizardEngine\src\core\Logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object WizardEngine/CMakeFiles/WizardEngine.dir/src/core/Logger.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\WizardEngine.dir\src\core\Logger.cpp.obj /FdCMakeFiles\WizardEngine.dir\WizardEngine.pdb /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\core\Logger.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WizardEngine.dir/src/core/Logger.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\WizardEngine.dir\src\core\Logger.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\core\Logger.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WizardEngine.dir/src/core/Logger.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\WizardEngine.dir\src\core\Logger.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\core\Logger.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\imgui\ImguiLayer.cpp.obj: WizardEngine\CMakeFiles\WizardEngine.dir\flags.make
WizardEngine\CMakeFiles\WizardEngine.dir\src\imgui\ImguiLayer.cpp.obj: ..\WizardEngine\src\imgui\ImguiLayer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object WizardEngine/CMakeFiles/WizardEngine.dir/src/imgui/ImguiLayer.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\WizardEngine.dir\src\imgui\ImguiLayer.cpp.obj /FdCMakeFiles\WizardEngine.dir\WizardEngine.pdb /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\imgui\ImguiLayer.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\imgui\ImguiLayer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WizardEngine.dir/src/imgui/ImguiLayer.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\WizardEngine.dir\src\imgui\ImguiLayer.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\imgui\ImguiLayer.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\imgui\ImguiLayer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WizardEngine.dir/src/imgui/ImguiLayer.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\WizardEngine.dir\src\imgui\ImguiLayer.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\imgui\ImguiLayer.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\network\Client.cpp.obj: WizardEngine\CMakeFiles\WizardEngine.dir\flags.make
WizardEngine\CMakeFiles\WizardEngine.dir\src\network\Client.cpp.obj: ..\WizardEngine\src\network\Client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object WizardEngine/CMakeFiles/WizardEngine.dir/src/network/Client.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\WizardEngine.dir\src\network\Client.cpp.obj /FdCMakeFiles\WizardEngine.dir\WizardEngine.pdb /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\network\Client.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\network\Client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WizardEngine.dir/src/network/Client.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\WizardEngine.dir\src\network\Client.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\network\Client.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\network\Client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WizardEngine.dir/src/network/Client.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\WizardEngine.dir\src\network\Client.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\network\Client.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\platform\windows\WindowsWindow.cpp.obj: WizardEngine\CMakeFiles\WizardEngine.dir\flags.make
WizardEngine\CMakeFiles\WizardEngine.dir\src\platform\windows\WindowsWindow.cpp.obj: ..\WizardEngine\src\platform\windows\WindowsWindow.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object WizardEngine/CMakeFiles/WizardEngine.dir/src/platform/windows/WindowsWindow.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\WizardEngine.dir\src\platform\windows\WindowsWindow.cpp.obj /FdCMakeFiles\WizardEngine.dir\WizardEngine.pdb /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\platform\windows\WindowsWindow.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\platform\windows\WindowsWindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WizardEngine.dir/src/platform/windows/WindowsWindow.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\WizardEngine.dir\src\platform\windows\WindowsWindow.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\platform\windows\WindowsWindow.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardEngine.dir\src\platform\windows\WindowsWindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WizardEngine.dir/src/platform/windows/WindowsWindow.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\WizardEngine.dir\src\platform\windows\WindowsWindow.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\platform\windows\WindowsWindow.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

# Object files for target WizardEngine
WizardEngine_OBJECTS = \
"CMakeFiles\WizardEngine.dir\src\Launcher.cpp.obj" \
"CMakeFiles\WizardEngine.dir\src\core\Application.cpp.obj" \
"CMakeFiles\WizardEngine.dir\src\core\Layer.cpp.obj" \
"CMakeFiles\WizardEngine.dir\src\core\LayerStack.cpp.obj" \
"CMakeFiles\WizardEngine.dir\src\core\Logger.cpp.obj" \
"CMakeFiles\WizardEngine.dir\src\imgui\ImguiLayer.cpp.obj" \
"CMakeFiles\WizardEngine.dir\src\network\Client.cpp.obj" \
"CMakeFiles\WizardEngine.dir\src\platform\windows\WindowsWindow.cpp.obj"

# External object files for target WizardEngine
WizardEngine_EXTERNAL_OBJECTS =

WizardEngine\WizardEngine.lib: WizardEngine\CMakeFiles\WizardEngine.dir\src\Launcher.cpp.obj
WizardEngine\WizardEngine.lib: WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Application.cpp.obj
WizardEngine\WizardEngine.lib: WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Layer.cpp.obj
WizardEngine\WizardEngine.lib: WizardEngine\CMakeFiles\WizardEngine.dir\src\core\LayerStack.cpp.obj
WizardEngine\WizardEngine.lib: WizardEngine\CMakeFiles\WizardEngine.dir\src\core\Logger.cpp.obj
WizardEngine\WizardEngine.lib: WizardEngine\CMakeFiles\WizardEngine.dir\src\imgui\ImguiLayer.cpp.obj
WizardEngine\WizardEngine.lib: WizardEngine\CMakeFiles\WizardEngine.dir\src\network\Client.cpp.obj
WizardEngine\WizardEngine.lib: WizardEngine\CMakeFiles\WizardEngine.dir\src\platform\windows\WindowsWindow.cpp.obj
WizardEngine\WizardEngine.lib: WizardEngine\CMakeFiles\WizardEngine.dir\build.make
WizardEngine\WizardEngine.lib: WizardEngine\CMakeFiles\WizardEngine.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX static library WizardEngine.lib"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	$(CMAKE_COMMAND) -P CMakeFiles\WizardEngine.dir\cmake_clean_target.cmake
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\lib.exe /nologo /machine:X86 /out:WizardEngine.lib @CMakeFiles\WizardEngine.dir\objects1.rsp 
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

# Rule to build all files generated by this target.
WizardEngine\CMakeFiles\WizardEngine.dir\build: WizardEngine\WizardEngine.lib
.PHONY : WizardEngine\CMakeFiles\WizardEngine.dir\build

WizardEngine\CMakeFiles\WizardEngine.dir\clean:
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	$(CMAKE_COMMAND) -P CMakeFiles\WizardEngine.dir\cmake_clean.cmake
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug
.PHONY : WizardEngine\CMakeFiles\WizardEngine.dir\clean

WizardEngine\CMakeFiles\WizardEngine.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\mecha\CLionProjects\Wizard C:\Users\mecha\CLionProjects\Wizard\WizardEngine C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine\CMakeFiles\WizardEngine.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : WizardEngine\CMakeFiles\WizardEngine.dir\depend

