# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cmake-build-release

# Include any dependencies generated for this target.
include CMakeFiles/pdm.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/pdm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pdm.dir/flags.make

CMakeFiles/pdm.dir/auth.cpp.o: CMakeFiles/pdm.dir/flags.make
CMakeFiles/pdm.dir/auth.cpp.o: ../auth.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pdm.dir/auth.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pdm.dir/auth.cpp.o -c /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/auth.cpp

CMakeFiles/pdm.dir/auth.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pdm.dir/auth.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/auth.cpp > CMakeFiles/pdm.dir/auth.cpp.i

CMakeFiles/pdm.dir/auth.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pdm.dir/auth.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/auth.cpp -o CMakeFiles/pdm.dir/auth.cpp.s

CMakeFiles/pdm.dir/cApp.cpp.o: CMakeFiles/pdm.dir/flags.make
CMakeFiles/pdm.dir/cApp.cpp.o: ../cApp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/pdm.dir/cApp.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pdm.dir/cApp.cpp.o -c /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cApp.cpp

CMakeFiles/pdm.dir/cApp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pdm.dir/cApp.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cApp.cpp > CMakeFiles/pdm.dir/cApp.cpp.i

CMakeFiles/pdm.dir/cApp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pdm.dir/cApp.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cApp.cpp -o CMakeFiles/pdm.dir/cApp.cpp.s

CMakeFiles/pdm.dir/cMain.cpp.o: CMakeFiles/pdm.dir/flags.make
CMakeFiles/pdm.dir/cMain.cpp.o: ../cMain.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/pdm.dir/cMain.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pdm.dir/cMain.cpp.o -c /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cMain.cpp

CMakeFiles/pdm.dir/cMain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pdm.dir/cMain.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cMain.cpp > CMakeFiles/pdm.dir/cMain.cpp.i

CMakeFiles/pdm.dir/cMain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pdm.dir/cMain.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cMain.cpp -o CMakeFiles/pdm.dir/cMain.cpp.s

CMakeFiles/pdm.dir/pdmFile.cpp.o: CMakeFiles/pdm.dir/flags.make
CMakeFiles/pdm.dir/pdmFile.cpp.o: ../pdmFile.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/pdm.dir/pdmFile.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pdm.dir/pdmFile.cpp.o -c /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/pdmFile.cpp

CMakeFiles/pdm.dir/pdmFile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pdm.dir/pdmFile.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/pdmFile.cpp > CMakeFiles/pdm.dir/pdmFile.cpp.i

CMakeFiles/pdm.dir/pdmFile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pdm.dir/pdmFile.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/pdmFile.cpp -o CMakeFiles/pdm.dir/pdmFile.cpp.s

CMakeFiles/pdm.dir/Tree_Ctrl.cpp.o: CMakeFiles/pdm.dir/flags.make
CMakeFiles/pdm.dir/Tree_Ctrl.cpp.o: ../Tree_Ctrl.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/pdm.dir/Tree_Ctrl.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pdm.dir/Tree_Ctrl.cpp.o -c /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/Tree_Ctrl.cpp

CMakeFiles/pdm.dir/Tree_Ctrl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pdm.dir/Tree_Ctrl.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/Tree_Ctrl.cpp > CMakeFiles/pdm.dir/Tree_Ctrl.cpp.i

CMakeFiles/pdm.dir/Tree_Ctrl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pdm.dir/Tree_Ctrl.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/Tree_Ctrl.cpp -o CMakeFiles/pdm.dir/Tree_Ctrl.cpp.s

CMakeFiles/pdm.dir/pdmrc.cpp.o: CMakeFiles/pdm.dir/flags.make
CMakeFiles/pdm.dir/pdmrc.cpp.o: ../pdmrc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/pdm.dir/pdmrc.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pdm.dir/pdmrc.cpp.o -c /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/pdmrc.cpp

CMakeFiles/pdm.dir/pdmrc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pdm.dir/pdmrc.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/pdmrc.cpp > CMakeFiles/pdm.dir/pdmrc.cpp.i

CMakeFiles/pdm.dir/pdmrc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pdm.dir/pdmrc.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/pdmrc.cpp -o CMakeFiles/pdm.dir/pdmrc.cpp.s

# Object files for target pdm
pdm_OBJECTS = \
"CMakeFiles/pdm.dir/auth.cpp.o" \
"CMakeFiles/pdm.dir/cApp.cpp.o" \
"CMakeFiles/pdm.dir/cMain.cpp.o" \
"CMakeFiles/pdm.dir/pdmFile.cpp.o" \
"CMakeFiles/pdm.dir/Tree_Ctrl.cpp.o" \
"CMakeFiles/pdm.dir/pdmrc.cpp.o"

# External object files for target pdm
pdm_EXTERNAL_OBJECTS =

pdm.app/Contents/MacOS/pdm: CMakeFiles/pdm.dir/auth.cpp.o
pdm.app/Contents/MacOS/pdm: CMakeFiles/pdm.dir/cApp.cpp.o
pdm.app/Contents/MacOS/pdm: CMakeFiles/pdm.dir/cMain.cpp.o
pdm.app/Contents/MacOS/pdm: CMakeFiles/pdm.dir/pdmFile.cpp.o
pdm.app/Contents/MacOS/pdm: CMakeFiles/pdm.dir/Tree_Ctrl.cpp.o
pdm.app/Contents/MacOS/pdm: CMakeFiles/pdm.dir/pdmrc.cpp.o
pdm.app/Contents/MacOS/pdm: CMakeFiles/pdm.dir/build.make
pdm.app/Contents/MacOS/pdm: CMakeFiles/pdm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable pdm.app/Contents/MacOS/pdm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pdm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pdm.dir/build: pdm.app/Contents/MacOS/pdm
.PHONY : CMakeFiles/pdm.dir/build

CMakeFiles/pdm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pdm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pdm.dir/clean

CMakeFiles/pdm.dir/depend:
	cd /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cmake-build-release /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cmake-build-release /Users/mikeyang/Downloads/wxWidgets-master/samples/minimal/cmake-build-release/CMakeFiles/pdm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pdm.dir/depend

