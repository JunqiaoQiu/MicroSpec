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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build

# Include any dependencies generated for this target.
include test/CMakeFiles/DFAcheckOnMultipleCores.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/DFAcheckOnMultipleCores.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/DFAcheckOnMultipleCores.dir/flags.make

test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o: test/CMakeFiles/DFAcheckOnMultipleCores.dir/flags.make
test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o: ../test/DFAcheckOnMultipleCores.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o"
	cd /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/test && /opt/rh/devtoolset-7/root/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o -c /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/test/DFAcheckOnMultipleCores.cpp

test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.i"
	cd /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/test && /opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/test/DFAcheckOnMultipleCores.cpp > CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.i

test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.s"
	cd /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/test && /opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/test/DFAcheckOnMultipleCores.cpp -o CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.s

test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o.requires:

.PHONY : test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o.requires

test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o.provides: test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/DFAcheckOnMultipleCores.dir/build.make test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o.provides.build
.PHONY : test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o.provides

test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o.provides.build: test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o


# Object files for target DFAcheckOnMultipleCores
DFAcheckOnMultipleCores_OBJECTS = \
"CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o"

# External object files for target DFAcheckOnMultipleCores
DFAcheckOnMultipleCores_EXTERNAL_OBJECTS =

test/DFAcheckOnMultipleCores: test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o
test/DFAcheckOnMultipleCores: test/CMakeFiles/DFAcheckOnMultipleCores.dir/build.make
test/DFAcheckOnMultipleCores: libMicroSpec.a
test/DFAcheckOnMultipleCores: test/CMakeFiles/DFAcheckOnMultipleCores.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable DFAcheckOnMultipleCores"
	cd /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DFAcheckOnMultipleCores.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/DFAcheckOnMultipleCores.dir/build: test/DFAcheckOnMultipleCores

.PHONY : test/CMakeFiles/DFAcheckOnMultipleCores.dir/build

test/CMakeFiles/DFAcheckOnMultipleCores.dir/requires: test/CMakeFiles/DFAcheckOnMultipleCores.dir/DFAcheckOnMultipleCores.cpp.o.requires

.PHONY : test/CMakeFiles/DFAcheckOnMultipleCores.dir/requires

test/CMakeFiles/DFAcheckOnMultipleCores.dir/clean:
	cd /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/test && $(CMAKE_COMMAND) -P CMakeFiles/DFAcheckOnMultipleCores.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/DFAcheckOnMultipleCores.dir/clean

test/CMakeFiles/DFAcheckOnMultipleCores.dir/depend:
	cd /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/test /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/test /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/test/CMakeFiles/DFAcheckOnMultipleCores.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/DFAcheckOnMultipleCores.dir/depend

