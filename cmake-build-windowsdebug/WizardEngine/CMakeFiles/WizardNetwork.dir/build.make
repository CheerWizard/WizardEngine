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
include WizardEngine\CMakeFiles\WizardNetwork.dir\depend.make
# Include the progress variables for this target.
include WizardEngine\CMakeFiles\WizardNetwork.dir\progress.make

# Include the compile flags for this target's objects.
include WizardEngine\CMakeFiles\WizardNetwork.dir\flags.make

WizardEngine\CMakeFiles\WizardNetwork.dir\src\network\Client.cpp.obj: WizardEngine\CMakeFiles\WizardNetwork.dir\flags.make
WizardEngine\CMakeFiles\WizardNetwork.dir\src\network\Client.cpp.obj: ..\WizardEngine\src\network\Client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object WizardEngine/CMakeFiles/WizardNetwork.dir/src/network/Client.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\WizardNetwork.dir\src\network\Client.cpp.obj /FdCMakeFiles\WizardNetwork.dir\ /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\network\Client.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardNetwork.dir\src\network\Client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WizardNetwork.dir/src/network/Client.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\WizardNetwork.dir\src\network\Client.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\network\Client.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\CMakeFiles\WizardNetwork.dir\src\network\Client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WizardNetwork.dir/src/network/Client.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\WizardNetwork.dir\src\network\Client.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\src\network\Client.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

# Object files for target WizardNetwork
WizardNetwork_OBJECTS = \
"CMakeFiles\WizardNetwork.dir\src\network\Client.cpp.obj"

# External object files for target WizardNetwork
WizardNetwork_EXTERNAL_OBJECTS =

WizardEngine\WizardNetwork.exe: WizardEngine\CMakeFiles\WizardNetwork.dir\src\network\Client.cpp.obj
WizardEngine\WizardNetwork.exe: WizardEngine\CMakeFiles\WizardNetwork.dir\build.make
WizardEngine\WizardNetwork.exe: WizardEngine\WizardEngine.lib
WizardEngine\WizardNetwork.exe: WizardEngine\vendor\spdlog\spdlogd.lib
WizardEngine\WizardNetwork.exe: WizardEngine\vendor\glfw\src\glfw3.lib
WizardEngine\WizardNetwork.exe: WizardEngine\CMakeFiles\WizardNetwork.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable WizardNetwork.exe"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	"C:\Program Files\JetBrains\CLion 2021.2\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\WizardNetwork.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100177~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100177~1.0\x86\mt.exe --manifests -- C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\WizardNetwork.dir\objects1.rsp @<<
 /out:WizardNetwork.exe /implib:WizardNetwork.lib /pdb:C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine\WizardNetwork.pdb /version:0.0 /machine:X86 /debug /INCREMENTAL /subsystem:console  WizardEngine.lib vendor\spdlog\spdlogd.lib vendor\glfw\src\glfw3.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

# Rule to build all files generated by this target.
WizardEngine\CMakeFiles\WizardNetwork.dir\build: WizardEngine\WizardNetwork.exe
.PHONY : WizardEngine\CMakeFiles\WizardNetwork.dir\build

WizardEngine\CMakeFiles\WizardNetwork.dir\clean:
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine
	$(CMAKE_COMMAND) -P CMakeFiles\WizardNetwork.dir\cmake_clean.cmake
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug
.PHONY : WizardEngine\CMakeFiles\WizardNetwork.dir\clean

WizardEngine\CMakeFiles\WizardNetwork.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\mecha\CLionProjects\Wizard C:\Users\mecha\CLionProjects\Wizard\WizardEngine C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine\CMakeFiles\WizardNetwork.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : WizardEngine\CMakeFiles\WizardNetwork.dir\depend

