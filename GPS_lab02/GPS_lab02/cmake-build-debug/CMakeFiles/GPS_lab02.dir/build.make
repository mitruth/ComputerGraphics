# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_SOURCE_DIR = /Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/GPS_lab02.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/GPS_lab02.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GPS_lab02.dir/flags.make

CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o: CMakeFiles/GPS_lab02.dir/flags.make
CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o: ../OpenGL_4_Application_VS2015.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o -c /Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02/OpenGL_4_Application_VS2015.cpp

CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02/OpenGL_4_Application_VS2015.cpp > CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.i

CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02/OpenGL_4_Application_VS2015.cpp -o CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.s

CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o.requires:

.PHONY : CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o.requires

CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o.provides: CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o.requires
	$(MAKE) -f CMakeFiles/GPS_lab02.dir/build.make CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o.provides.build
.PHONY : CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o.provides

CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o.provides.build: CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o


# Object files for target GPS_lab02
GPS_lab02_OBJECTS = \
"CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o"

# External object files for target GPS_lab02
GPS_lab02_EXTERNAL_OBJECTS =

GPS_lab02: CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o
GPS_lab02: CMakeFiles/GPS_lab02.dir/build.make
GPS_lab02: CMakeFiles/GPS_lab02.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable GPS_lab02"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GPS_lab02.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GPS_lab02.dir/build: GPS_lab02

.PHONY : CMakeFiles/GPS_lab02.dir/build

CMakeFiles/GPS_lab02.dir/requires: CMakeFiles/GPS_lab02.dir/OpenGL_4_Application_VS2015.cpp.o.requires

.PHONY : CMakeFiles/GPS_lab02.dir/requires

CMakeFiles/GPS_lab02.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GPS_lab02.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GPS_lab02.dir/clean

CMakeFiles/GPS_lab02.dir/depend:
	cd /Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02 /Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02 /Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02/cmake-build-debug /Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02/cmake-build-debug /Users/Mitruth/Documents/GPS/GPS_lab02/GPS_lab02/cmake-build-debug/CMakeFiles/GPS_lab02.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/GPS_lab02.dir/depend

