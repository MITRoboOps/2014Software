# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/roboops/groovycat/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/roboops/groovycat/build

# Utility rule file for robot_upstart_tmpl.

# Include the progress variables for this target.
include robot_upstart-hydro-devel/CMakeFiles/robot_upstart_tmpl.dir/progress.make

robot_upstart-hydro-devel/CMakeFiles/robot_upstart_tmpl: robot_upstart-hydro-devel/lib/robot_upstart/tmpl

robot_upstart-hydro-devel/lib/robot_upstart/tmpl: robot_upstart-hydro-devel/tmpl/start_gen
robot_upstart-hydro-devel/lib/robot_upstart/tmpl: robot_upstart-hydro-devel/tmpl/stop_gen
robot_upstart-hydro-devel/lib/robot_upstart/tmpl: robot_upstart-hydro-devel/tmpl/job_conf_gen
	$(CMAKE_COMMAND) -E cmake_progress_report /home/roboops/groovycat/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating lib/robot_upstart/tmpl"
	cd /home/roboops/groovycat/devel && /usr/bin/cmake -E copy_directory /home/roboops/groovycat/build/robot_upstart-hydro-devel/tmpl lib/robot_upstart/tmpl

robot_upstart-hydro-devel/tmpl/start_gen: /home/roboops/groovycat/src/robot_upstart-hydro-devel/tmpl/start
	$(CMAKE_COMMAND) -E cmake_progress_report /home/roboops/groovycat/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating tmpl/start_gen"
	cd /home/roboops/groovycat/build/robot_upstart-hydro-devel && mkdir -p tmpl
	cd /home/roboops/groovycat/build/robot_upstart-hydro-devel && /home/roboops/groovycat/src/robot_upstart-hydro-devel/scripts/mktmpl /home/roboops/groovycat/src/robot_upstart-hydro-devel/tmpl/start tmpl/start_gen

robot_upstart-hydro-devel/tmpl/stop_gen: /home/roboops/groovycat/src/robot_upstart-hydro-devel/tmpl/stop
	$(CMAKE_COMMAND) -E cmake_progress_report /home/roboops/groovycat/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating tmpl/stop_gen"
	cd /home/roboops/groovycat/build/robot_upstart-hydro-devel && mkdir -p tmpl
	cd /home/roboops/groovycat/build/robot_upstart-hydro-devel && /home/roboops/groovycat/src/robot_upstart-hydro-devel/scripts/mktmpl /home/roboops/groovycat/src/robot_upstart-hydro-devel/tmpl/stop tmpl/stop_gen

robot_upstart-hydro-devel/tmpl/job_conf_gen: /home/roboops/groovycat/src/robot_upstart-hydro-devel/tmpl/job_conf
	$(CMAKE_COMMAND) -E cmake_progress_report /home/roboops/groovycat/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating tmpl/job_conf_gen"
	cd /home/roboops/groovycat/build/robot_upstart-hydro-devel && mkdir -p tmpl
	cd /home/roboops/groovycat/build/robot_upstart-hydro-devel && /home/roboops/groovycat/src/robot_upstart-hydro-devel/scripts/mktmpl /home/roboops/groovycat/src/robot_upstart-hydro-devel/tmpl/job_conf tmpl/job_conf_gen

robot_upstart_tmpl: robot_upstart-hydro-devel/CMakeFiles/robot_upstart_tmpl
robot_upstart_tmpl: robot_upstart-hydro-devel/lib/robot_upstart/tmpl
robot_upstart_tmpl: robot_upstart-hydro-devel/tmpl/start_gen
robot_upstart_tmpl: robot_upstart-hydro-devel/tmpl/stop_gen
robot_upstart_tmpl: robot_upstart-hydro-devel/tmpl/job_conf_gen
robot_upstart_tmpl: robot_upstart-hydro-devel/CMakeFiles/robot_upstart_tmpl.dir/build.make
.PHONY : robot_upstart_tmpl

# Rule to build all files generated by this target.
robot_upstart-hydro-devel/CMakeFiles/robot_upstart_tmpl.dir/build: robot_upstart_tmpl
.PHONY : robot_upstart-hydro-devel/CMakeFiles/robot_upstart_tmpl.dir/build

robot_upstart-hydro-devel/CMakeFiles/robot_upstart_tmpl.dir/clean:
	cd /home/roboops/groovycat/build/robot_upstart-hydro-devel && $(CMAKE_COMMAND) -P CMakeFiles/robot_upstart_tmpl.dir/cmake_clean.cmake
.PHONY : robot_upstart-hydro-devel/CMakeFiles/robot_upstart_tmpl.dir/clean

robot_upstart-hydro-devel/CMakeFiles/robot_upstart_tmpl.dir/depend:
	cd /home/roboops/groovycat/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/roboops/groovycat/src /home/roboops/groovycat/src/robot_upstart-hydro-devel /home/roboops/groovycat/build /home/roboops/groovycat/build/robot_upstart-hydro-devel /home/roboops/groovycat/build/robot_upstart-hydro-devel/CMakeFiles/robot_upstart_tmpl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : robot_upstart-hydro-devel/CMakeFiles/robot_upstart_tmpl.dir/depend

