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
CMAKE_BINARY_DIR = C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease

# Include any dependencies generated for this target.
include WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\depend.make
# Include the progress variables for this target.
include WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\progress.make

# Include the compile flags for this target's objects.
include WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\flags.make

WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\chrono.cpp.obj: WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\flags.make
WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\chrono.cpp.obj: ..\WizardEngine\vendor\boost\chrono\src\chrono.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object WizardEngine/vendor/boost/chrono/CMakeFiles/boost_chrono.dir/src/chrono.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\boost_chrono.dir\src\chrono.cpp.obj /FdCMakeFiles\boost_chrono.dir\boost_chrono.pdb /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\chrono\src\chrono.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease

WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\chrono.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_chrono.dir/src/chrono.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\boost_chrono.dir\src\chrono.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\chrono\src\chrono.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease

WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\chrono.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_chrono.dir/src/chrono.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\boost_chrono.dir\src\chrono.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\chrono\src\chrono.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease

WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\process_cpu_clocks.cpp.obj: WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\flags.make
WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\process_cpu_clocks.cpp.obj: ..\WizardEngine\vendor\boost\chrono\src\process_cpu_clocks.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object WizardEngine/vendor/boost/chrono/CMakeFiles/boost_chrono.dir/src/process_cpu_clocks.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\boost_chrono.dir\src\process_cpu_clocks.cpp.obj /FdCMakeFiles\boost_chrono.dir\boost_chrono.pdb /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\chrono\src\process_cpu_clocks.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease

WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\process_cpu_clocks.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_chrono.dir/src/process_cpu_clocks.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\boost_chrono.dir\src\process_cpu_clocks.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\chrono\src\process_cpu_clocks.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease

WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\process_cpu_clocks.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_chrono.dir/src/process_cpu_clocks.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\boost_chrono.dir\src\process_cpu_clocks.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\chrono\src\process_cpu_clocks.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease

WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\thread_clock.cpp.obj: WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\flags.make
WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\thread_clock.cpp.obj: ..\WizardEngine\vendor\boost\chrono\src\thread_clock.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object WizardEngine/vendor/boost/chrono/CMakeFiles/boost_chrono.dir/src/thread_clock.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\boost_chrono.dir\src\thread_clock.cpp.obj /FdCMakeFiles\boost_chrono.dir\boost_chrono.pdb /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\chrono\src\thread_clock.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease

WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\thread_clock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_chrono.dir/src/thread_clock.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\boost_chrono.dir\src\thread_clock.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\chrono\src\thread_clock.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease

WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\thread_clock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_chrono.dir/src/thread_clock.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\boost_chrono.dir\src\thread_clock.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\chrono\src\thread_clock.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease

# Object files for target boost_chrono
boost_chrono_OBJECTS = \
"CMakeFiles\boost_chrono.dir\src\chrono.cpp.obj" \
"CMakeFiles\boost_chrono.dir\src\process_cpu_clocks.cpp.obj" \
"CMakeFiles\boost_chrono.dir\src\thread_clock.cpp.obj"

# External object files for target boost_chrono
boost_chrono_EXTERNAL_OBJECTS =

WizardEngine\vendor\boost\chrono\boost_chrono.lib: WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\chrono.cpp.obj
WizardEngine\vendor\boost\chrono\boost_chrono.lib: WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\process_cpu_clocks.cpp.obj
WizardEngine\vendor\boost\chrono\boost_chrono.lib: WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\src\thread_clock.cpp.obj
WizardEngine\vendor\boost\chrono\boost_chrono.lib: WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\build.make
WizardEngine\vendor\boost\chrono\boost_chrono.lib: WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library boost_chrono.lib"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono
	$(CMAKE_COMMAND) -P CMakeFiles\boost_chrono.dir\cmake_clean_target.cmake
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\lib.exe /nologo /machine:X86 /out:boost_chrono.lib @CMakeFiles\boost_chrono.dir\objects1.rsp 
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease

# Rule to build all files generated by this target.
WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\build: WizardEngine\vendor\boost\chrono\boost_chrono.lib
.PHONY : WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\build

WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\clean:
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono
	$(CMAKE_COMMAND) -P CMakeFiles\boost_chrono.dir\cmake_clean.cmake
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease
.PHONY : WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\clean

WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\mecha\CLionProjects\Wizard C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\chrono C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsrelease\WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : WizardEngine\vendor\boost\chrono\CMakeFiles\boost_chrono.dir\depend

