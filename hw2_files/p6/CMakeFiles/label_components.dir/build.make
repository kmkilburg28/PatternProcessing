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
include CMakeFiles/label_components.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/label_components.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/label_components.dir/flags.make

CMakeFiles/label_components.dir/home/kadenk/HCI575/HW2/hw2_files/label_components.cpp.o: CMakeFiles/label_components.dir/flags.make
CMakeFiles/label_components.dir/home/kadenk/HCI575/HW2/hw2_files/label_components.cpp.o: /home/kadenk/HCI575/HW2/hw2_files/label_components.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kadenk/HCI575/HW2/hw2_files/p6/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/label_components.dir/home/kadenk/HCI575/HW2/hw2_files/label_components.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/label_components.dir/home/kadenk/HCI575/HW2/hw2_files/label_components.cpp.o -c /home/kadenk/HCI575/HW2/hw2_files/label_components.cpp

CMakeFiles/label_components.dir/home/kadenk/HCI575/HW2/hw2_files/label_components.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/label_components.dir/home/kadenk/HCI575/HW2/hw2_files/label_components.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kadenk/HCI575/HW2/hw2_files/label_components.cpp > CMakeFiles/label_components.dir/home/kadenk/HCI575/HW2/hw2_files/label_components.cpp.i

CMakeFiles/label_components.dir/home/kadenk/HCI575/HW2/hw2_files/label_components.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/label_components.dir/home/kadenk/HCI575/HW2/hw2_files/label_components.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kadenk/HCI575/HW2/hw2_files/label_components.cpp -o CMakeFiles/label_components.dir/home/kadenk/HCI575/HW2/hw2_files/label_components.cpp.s

# Object files for target label_components
label_components_OBJECTS = \
"CMakeFiles/label_components.dir/home/kadenk/HCI575/HW2/hw2_files/label_components.cpp.o"

# External object files for target label_components
label_components_EXTERNAL_OBJECTS =

liblabel_components.a: CMakeFiles/label_components.dir/home/kadenk/HCI575/HW2/hw2_files/label_components.cpp.o
liblabel_components.a: CMakeFiles/label_components.dir/build.make
liblabel_components.a: CMakeFiles/label_components.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kadenk/HCI575/HW2/hw2_files/p6/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library liblabel_components.a"
	$(CMAKE_COMMAND) -P CMakeFiles/label_components.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/label_components.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/label_components.dir/build: liblabel_components.a

.PHONY : CMakeFiles/label_components.dir/build

CMakeFiles/label_components.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/label_components.dir/cmake_clean.cmake
.PHONY : CMakeFiles/label_components.dir/clean

CMakeFiles/label_components.dir/depend:
	cd /home/kadenk/HCI575/HW2/hw2_files/p6 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kadenk/HCI575/HW2/hw2_files/p6 /home/kadenk/HCI575/HW2/hw2_files/p6 /home/kadenk/HCI575/HW2/hw2_files/p6 /home/kadenk/HCI575/HW2/hw2_files/p6 /home/kadenk/HCI575/HW2/hw2_files/p6/CMakeFiles/label_components.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/label_components.dir/depend

