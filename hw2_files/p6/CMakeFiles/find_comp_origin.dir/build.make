# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kadenk/HCI575/HW2/hw2_files/p6

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kadenk/HCI575/HW2/hw2_files/p6

# Include any dependencies generated for this target.
include CMakeFiles/find_comp_origin.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/find_comp_origin.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/find_comp_origin.dir/flags.make

CMakeFiles/find_comp_origin.dir/home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp.o: CMakeFiles/find_comp_origin.dir/flags.make
CMakeFiles/find_comp_origin.dir/home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp.o: /home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kadenk/HCI575/HW2/hw2_files/p6/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/find_comp_origin.dir/home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/find_comp_origin.dir/home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp.o -c /home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp

CMakeFiles/find_comp_origin.dir/home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/find_comp_origin.dir/home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp > CMakeFiles/find_comp_origin.dir/home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp.i

CMakeFiles/find_comp_origin.dir/home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/find_comp_origin.dir/home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp -o CMakeFiles/find_comp_origin.dir/home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp.s

# Object files for target find_comp_origin
find_comp_origin_OBJECTS = \
"CMakeFiles/find_comp_origin.dir/home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp.o"

# External object files for target find_comp_origin
find_comp_origin_EXTERNAL_OBJECTS =

libfind_comp_origin.a: CMakeFiles/find_comp_origin.dir/home/kadenk/HCI575/HW2/hw2_files/find_comp_origin.cpp.o
libfind_comp_origin.a: CMakeFiles/find_comp_origin.dir/build.make
libfind_comp_origin.a: CMakeFiles/find_comp_origin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kadenk/HCI575/HW2/hw2_files/p6/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libfind_comp_origin.a"
	$(CMAKE_COMMAND) -P CMakeFiles/find_comp_origin.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/find_comp_origin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/find_comp_origin.dir/build: libfind_comp_origin.a

.PHONY : CMakeFiles/find_comp_origin.dir/build

CMakeFiles/find_comp_origin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/find_comp_origin.dir/cmake_clean.cmake
.PHONY : CMakeFiles/find_comp_origin.dir/clean

CMakeFiles/find_comp_origin.dir/depend:
	cd /home/kadenk/HCI575/HW2/hw2_files/p6 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kadenk/HCI575/HW2/hw2_files/p6 /home/kadenk/HCI575/HW2/hw2_files/p6 /home/kadenk/HCI575/HW2/hw2_files/p6 /home/kadenk/HCI575/HW2/hw2_files/p6 /home/kadenk/HCI575/HW2/hw2_files/p6/CMakeFiles/find_comp_origin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/find_comp_origin.dir/depend

