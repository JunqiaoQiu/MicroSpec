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
include CMakeFiles/MicroSpec.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MicroSpec.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MicroSpec.dir/flags.make

CMakeFiles/MicroSpec.dir/src/Action.cpp.o: CMakeFiles/MicroSpec.dir/flags.make
CMakeFiles/MicroSpec.dir/src/Action.cpp.o: ../src/Action.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MicroSpec.dir/src/Action.cpp.o"
	/opt/rh/devtoolset-7/root/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MicroSpec.dir/src/Action.cpp.o -c /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/Action.cpp

CMakeFiles/MicroSpec.dir/src/Action.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MicroSpec.dir/src/Action.cpp.i"
	/opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/Action.cpp > CMakeFiles/MicroSpec.dir/src/Action.cpp.i

CMakeFiles/MicroSpec.dir/src/Action.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MicroSpec.dir/src/Action.cpp.s"
	/opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/Action.cpp -o CMakeFiles/MicroSpec.dir/src/Action.cpp.s

CMakeFiles/MicroSpec.dir/src/Action.cpp.o.requires:

.PHONY : CMakeFiles/MicroSpec.dir/src/Action.cpp.o.requires

CMakeFiles/MicroSpec.dir/src/Action.cpp.o.provides: CMakeFiles/MicroSpec.dir/src/Action.cpp.o.requires
	$(MAKE) -f CMakeFiles/MicroSpec.dir/build.make CMakeFiles/MicroSpec.dir/src/Action.cpp.o.provides.build
.PHONY : CMakeFiles/MicroSpec.dir/src/Action.cpp.o.provides

CMakeFiles/MicroSpec.dir/src/Action.cpp.o.provides.build: CMakeFiles/MicroSpec.dir/src/Action.cpp.o


CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o: CMakeFiles/MicroSpec.dir/flags.make
CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o: ../src/MicroSpec.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o"
	/opt/rh/devtoolset-7/root/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o -c /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/MicroSpec.cpp

CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.i"
	/opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/MicroSpec.cpp > CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.i

CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.s"
	/opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/MicroSpec.cpp -o CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.s

CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o.requires:

.PHONY : CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o.requires

CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o.provides: CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o.requires
	$(MAKE) -f CMakeFiles/MicroSpec.dir/build.make CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o.provides.build
.PHONY : CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o.provides

CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o.provides.build: CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o


CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o: CMakeFiles/MicroSpec.dir/flags.make
CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o: ../src/MicroSpecPthread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o"
	/opt/rh/devtoolset-7/root/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o -c /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/MicroSpecPthread.cpp

CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.i"
	/opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/MicroSpecPthread.cpp > CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.i

CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.s"
	/opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/MicroSpecPthread.cpp -o CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.s

CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o.requires:

.PHONY : CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o.requires

CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o.provides: CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o.requires
	$(MAKE) -f CMakeFiles/MicroSpec.dir/build.make CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o.provides.build
.PHONY : CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o.provides

CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o.provides.build: CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o


CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o: CMakeFiles/MicroSpec.dir/flags.make
CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o: ../src/Predictor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o"
	/opt/rh/devtoolset-7/root/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o -c /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/Predictor.cpp

CMakeFiles/MicroSpec.dir/src/Predictor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MicroSpec.dir/src/Predictor.cpp.i"
	/opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/Predictor.cpp > CMakeFiles/MicroSpec.dir/src/Predictor.cpp.i

CMakeFiles/MicroSpec.dir/src/Predictor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MicroSpec.dir/src/Predictor.cpp.s"
	/opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/Predictor.cpp -o CMakeFiles/MicroSpec.dir/src/Predictor.cpp.s

CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o.requires:

.PHONY : CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o.requires

CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o.provides: CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o.requires
	$(MAKE) -f CMakeFiles/MicroSpec.dir/build.make CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o.provides.build
.PHONY : CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o.provides

CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o.provides.build: CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o


CMakeFiles/MicroSpec.dir/src/Resources.cpp.o: CMakeFiles/MicroSpec.dir/flags.make
CMakeFiles/MicroSpec.dir/src/Resources.cpp.o: ../src/Resources.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/MicroSpec.dir/src/Resources.cpp.o"
	/opt/rh/devtoolset-7/root/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MicroSpec.dir/src/Resources.cpp.o -c /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/Resources.cpp

CMakeFiles/MicroSpec.dir/src/Resources.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MicroSpec.dir/src/Resources.cpp.i"
	/opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/Resources.cpp > CMakeFiles/MicroSpec.dir/src/Resources.cpp.i

CMakeFiles/MicroSpec.dir/src/Resources.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MicroSpec.dir/src/Resources.cpp.s"
	/opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/Resources.cpp -o CMakeFiles/MicroSpec.dir/src/Resources.cpp.s

CMakeFiles/MicroSpec.dir/src/Resources.cpp.o.requires:

.PHONY : CMakeFiles/MicroSpec.dir/src/Resources.cpp.o.requires

CMakeFiles/MicroSpec.dir/src/Resources.cpp.o.provides: CMakeFiles/MicroSpec.dir/src/Resources.cpp.o.requires
	$(MAKE) -f CMakeFiles/MicroSpec.dir/build.make CMakeFiles/MicroSpec.dir/src/Resources.cpp.o.provides.build
.PHONY : CMakeFiles/MicroSpec.dir/src/Resources.cpp.o.provides

CMakeFiles/MicroSpec.dir/src/Resources.cpp.o.provides.build: CMakeFiles/MicroSpec.dir/src/Resources.cpp.o


CMakeFiles/MicroSpec.dir/src/Timer.cpp.o: CMakeFiles/MicroSpec.dir/flags.make
CMakeFiles/MicroSpec.dir/src/Timer.cpp.o: ../src/Timer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/MicroSpec.dir/src/Timer.cpp.o"
	/opt/rh/devtoolset-7/root/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MicroSpec.dir/src/Timer.cpp.o -c /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/Timer.cpp

CMakeFiles/MicroSpec.dir/src/Timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MicroSpec.dir/src/Timer.cpp.i"
	/opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/Timer.cpp > CMakeFiles/MicroSpec.dir/src/Timer.cpp.i

CMakeFiles/MicroSpec.dir/src/Timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MicroSpec.dir/src/Timer.cpp.s"
	/opt/rh/devtoolset-7/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/src/Timer.cpp -o CMakeFiles/MicroSpec.dir/src/Timer.cpp.s

CMakeFiles/MicroSpec.dir/src/Timer.cpp.o.requires:

.PHONY : CMakeFiles/MicroSpec.dir/src/Timer.cpp.o.requires

CMakeFiles/MicroSpec.dir/src/Timer.cpp.o.provides: CMakeFiles/MicroSpec.dir/src/Timer.cpp.o.requires
	$(MAKE) -f CMakeFiles/MicroSpec.dir/build.make CMakeFiles/MicroSpec.dir/src/Timer.cpp.o.provides.build
.PHONY : CMakeFiles/MicroSpec.dir/src/Timer.cpp.o.provides

CMakeFiles/MicroSpec.dir/src/Timer.cpp.o.provides.build: CMakeFiles/MicroSpec.dir/src/Timer.cpp.o


# Object files for target MicroSpec
MicroSpec_OBJECTS = \
"CMakeFiles/MicroSpec.dir/src/Action.cpp.o" \
"CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o" \
"CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o" \
"CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o" \
"CMakeFiles/MicroSpec.dir/src/Resources.cpp.o" \
"CMakeFiles/MicroSpec.dir/src/Timer.cpp.o"

# External object files for target MicroSpec
MicroSpec_EXTERNAL_OBJECTS =

libMicroSpec.a: CMakeFiles/MicroSpec.dir/src/Action.cpp.o
libMicroSpec.a: CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o
libMicroSpec.a: CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o
libMicroSpec.a: CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o
libMicroSpec.a: CMakeFiles/MicroSpec.dir/src/Resources.cpp.o
libMicroSpec.a: CMakeFiles/MicroSpec.dir/src/Timer.cpp.o
libMicroSpec.a: CMakeFiles/MicroSpec.dir/build.make
libMicroSpec.a: CMakeFiles/MicroSpec.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX static library libMicroSpec.a"
	$(CMAKE_COMMAND) -P CMakeFiles/MicroSpec.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MicroSpec.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MicroSpec.dir/build: libMicroSpec.a

.PHONY : CMakeFiles/MicroSpec.dir/build

CMakeFiles/MicroSpec.dir/requires: CMakeFiles/MicroSpec.dir/src/Action.cpp.o.requires
CMakeFiles/MicroSpec.dir/requires: CMakeFiles/MicroSpec.dir/src/MicroSpec.cpp.o.requires
CMakeFiles/MicroSpec.dir/requires: CMakeFiles/MicroSpec.dir/src/MicroSpecPthread.cpp.o.requires
CMakeFiles/MicroSpec.dir/requires: CMakeFiles/MicroSpec.dir/src/Predictor.cpp.o.requires
CMakeFiles/MicroSpec.dir/requires: CMakeFiles/MicroSpec.dir/src/Resources.cpp.o.requires
CMakeFiles/MicroSpec.dir/requires: CMakeFiles/MicroSpec.dir/src/Timer.cpp.o.requires

.PHONY : CMakeFiles/MicroSpec.dir/requires

CMakeFiles/MicroSpec.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MicroSpec.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MicroSpec.dir/clean

CMakeFiles/MicroSpec.dir/depend:
	cd /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build /home/jqiu004/Desktop/Project/RepositoryRefactoring/MicroSpecRepo/build/CMakeFiles/MicroSpec.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MicroSpec.dir/depend

