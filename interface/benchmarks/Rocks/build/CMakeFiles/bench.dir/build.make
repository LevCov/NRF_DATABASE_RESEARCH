# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /home/anx_tl/anaconda3/lib/python3.11/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/anx_tl/anaconda3/lib/python3.11/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks/build

# Include any dependencies generated for this target.
include CMakeFiles/bench.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/bench.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/bench.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bench.dir/flags.make

CMakeFiles/bench.dir/bench_rocks.cpp.o: CMakeFiles/bench.dir/flags.make
CMakeFiles/bench.dir/bench_rocks.cpp.o: /home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks/bench_rocks.cpp
CMakeFiles/bench.dir/bench_rocks.cpp.o: CMakeFiles/bench.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bench.dir/bench_rocks.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bench.dir/bench_rocks.cpp.o -MF CMakeFiles/bench.dir/bench_rocks.cpp.o.d -o CMakeFiles/bench.dir/bench_rocks.cpp.o -c /home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks/bench_rocks.cpp

CMakeFiles/bench.dir/bench_rocks.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/bench.dir/bench_rocks.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks/bench_rocks.cpp > CMakeFiles/bench.dir/bench_rocks.cpp.i

CMakeFiles/bench.dir/bench_rocks.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/bench.dir/bench_rocks.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks/bench_rocks.cpp -o CMakeFiles/bench.dir/bench_rocks.cpp.s

# Object files for target bench
bench_OBJECTS = \
"CMakeFiles/bench.dir/bench_rocks.cpp.o"

# External object files for target bench
bench_EXTERNAL_OBJECTS =

bench: CMakeFiles/bench.dir/bench_rocks.cpp.o
bench: CMakeFiles/bench.dir/build.make
bench: build/RocksDB/libRocks.a
bench: CMakeFiles/bench.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bench"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bench.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bench.dir/build: bench
.PHONY : CMakeFiles/bench.dir/build

CMakeFiles/bench.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bench.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bench.dir/clean

CMakeFiles/bench.dir/depend:
	cd /home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks /home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks /home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks/build /home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks/build /home/anx_tl/cplusplus/project_nrf/NRF_DATABASE_RESEARCH/interface/benchmarks/Rocks/build/CMakeFiles/bench.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/bench.dir/depend
