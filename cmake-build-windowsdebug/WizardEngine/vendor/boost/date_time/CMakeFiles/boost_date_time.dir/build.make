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
include WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\depend.make
# Include the progress variables for this target.
include WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\progress.make

# Include the compile flags for this target's objects.
include WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\flags.make

WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\src\gregorian\greg_month.cpp.obj: WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\flags.make
WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\src\gregorian\greg_month.cpp.obj: ..\WizardEngine\vendor\boost\date_time\src\gregorian\greg_month.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object WizardEngine/vendor/boost/date_time/CMakeFiles/boost_date_time.dir/src/gregorian/greg_month.cpp.obj"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine\vendor\boost\date_time
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\boost_date_time.dir\src\gregorian\greg_month.cpp.obj /FdCMakeFiles\boost_date_time.dir\boost_date_time.pdb /FS -c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\date_time\src\gregorian\greg_month.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\src\gregorian\greg_month.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boost_date_time.dir/src/gregorian/greg_month.cpp.i"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine\vendor\boost\date_time
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\boost_date_time.dir\src\gregorian\greg_month.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\date_time\src\gregorian\greg_month.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\src\gregorian\greg_month.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boost_date_time.dir/src/gregorian/greg_month.cpp.s"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine\vendor\boost\date_time
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\boost_date_time.dir\src\gregorian\greg_month.cpp.s /c C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\date_time\src\gregorian\greg_month.cpp
<<
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

# Object files for target boost_date_time
boost_date_time_OBJECTS = \
"CMakeFiles\boost_date_time.dir\src\gregorian\greg_month.cpp.obj"

# External object files for target boost_date_time
boost_date_time_EXTERNAL_OBJECTS =

WizardEngine\vendor\boost\date_time\boost_date_time.lib: WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\src\gregorian\greg_month.cpp.obj
WizardEngine\vendor\boost\date_time\boost_date_time.lib: WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\build.make
WizardEngine\vendor\boost\date_time\boost_date_time.lib: WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library boost_date_time.lib"
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine\vendor\boost\date_time
	$(CMAKE_COMMAND) -P CMakeFiles\boost_date_time.dir\cmake_clean_target.cmake
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine\vendor\boost\date_time
	C:\PROGRA~2\MICROS~2\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\lib.exe /nologo /machine:X86 /out:boost_date_time.lib @CMakeFiles\boost_date_time.dir\objects1.rsp 
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug

# Rule to build all files generated by this target.
WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\build: WizardEngine\vendor\boost\date_time\boost_date_time.lib
.PHONY : WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\build

WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\clean:
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine\vendor\boost\date_time
	$(CMAKE_COMMAND) -P CMakeFiles\boost_date_time.dir\cmake_clean.cmake
	cd C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug
.PHONY : WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\clean

WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\mecha\CLionProjects\Wizard C:\Users\mecha\CLionProjects\Wizard\WizardEngine\vendor\boost\date_time C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine\vendor\boost\date_time C:\Users\mecha\CLionProjects\Wizard\cmake-build-windowsdebug\WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : WizardEngine\vendor\boost\date_time\CMakeFiles\boost_date_time.dir\depend

