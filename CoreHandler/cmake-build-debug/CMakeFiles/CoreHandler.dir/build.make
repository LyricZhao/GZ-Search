# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/CoreHandler.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CoreHandler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CoreHandler.dir/flags.make

CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o: CMakeFiles/CoreHandler.dir/flags.make
CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o: ../CoreHandler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o -c "/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler/CoreHandler.cpp"

CMakeFiles/CoreHandler.dir/CoreHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CoreHandler.dir/CoreHandler.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler/CoreHandler.cpp" > CMakeFiles/CoreHandler.dir/CoreHandler.cpp.i

CMakeFiles/CoreHandler.dir/CoreHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CoreHandler.dir/CoreHandler.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler/CoreHandler.cpp" -o CMakeFiles/CoreHandler.dir/CoreHandler.cpp.s

CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o.requires:

.PHONY : CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o.requires

CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o.provides: CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o.requires
	$(MAKE) -f CMakeFiles/CoreHandler.dir/build.make CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o.provides.build
.PHONY : CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o.provides

CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o.provides.build: CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o


# Object files for target CoreHandler
CoreHandler_OBJECTS = \
"CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o"

# External object files for target CoreHandler
CoreHandler_EXTERNAL_OBJECTS =

CoreHandler: CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o
CoreHandler: CMakeFiles/CoreHandler.dir/build.make
CoreHandler: CMakeFiles/CoreHandler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CoreHandler"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CoreHandler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CoreHandler.dir/build: CoreHandler

.PHONY : CMakeFiles/CoreHandler.dir/build

CMakeFiles/CoreHandler.dir/requires: CMakeFiles/CoreHandler.dir/CoreHandler.cpp.o.requires

.PHONY : CMakeFiles/CoreHandler.dir/requires

CMakeFiles/CoreHandler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CoreHandler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CoreHandler.dir/clean

CMakeFiles/CoreHandler.dir/depend:
	cd "/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler" "/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler" "/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler/cmake-build-debug" "/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler/cmake-build-debug" "/Users/stdafx/Desktop/Assignment/GZ Searcher/CoreHandler/cmake-build-debug/CMakeFiles/CoreHandler.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/CoreHandler.dir/depend

