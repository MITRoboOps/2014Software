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
CMAKE_SOURCE_DIR = /home/roboops/2014Software/groovy2014/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/roboops/2014Software/groovy2014/build

# Include any dependencies generated for this target.
include DriveTrain/CMakeFiles/driveTrain.dir/depend.make

# Include the progress variables for this target.
include DriveTrain/CMakeFiles/driveTrain.dir/progress.make

# Include the compile flags for this target's objects.
include DriveTrain/CMakeFiles/driveTrain.dir/flags.make

DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o: DriveTrain/CMakeFiles/driveTrain.dir/flags.make
DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o: /home/roboops/2014Software/groovy2014/src/DriveTrain/src/DriveTrainNode.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/roboops/2014Software/groovy2014/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o -c /home/roboops/2014Software/groovy2014/src/DriveTrain/src/DriveTrainNode.cpp

DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.i"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/roboops/2014Software/groovy2014/src/DriveTrain/src/DriveTrainNode.cpp > CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.i

DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.s"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/roboops/2014Software/groovy2014/src/DriveTrain/src/DriveTrainNode.cpp -o CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.s

DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o.requires:
.PHONY : DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o.requires

DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o.provides: DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o.requires
	$(MAKE) -f DriveTrain/CMakeFiles/driveTrain.dir/build.make DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o.provides.build
.PHONY : DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o.provides

DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o.provides.build: DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o

DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o: DriveTrain/CMakeFiles/driveTrain.dir/flags.make
DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o: /home/roboops/2014Software/groovy2014/src/DriveTrain/src/DriveTrainClass.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/roboops/2014Software/groovy2014/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o -c /home/roboops/2014Software/groovy2014/src/DriveTrain/src/DriveTrainClass.cpp

DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.i"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/roboops/2014Software/groovy2014/src/DriveTrain/src/DriveTrainClass.cpp > CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.i

DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.s"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/roboops/2014Software/groovy2014/src/DriveTrain/src/DriveTrainClass.cpp -o CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.s

DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o.requires:
.PHONY : DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o.requires

DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o.provides: DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o.requires
	$(MAKE) -f DriveTrain/CMakeFiles/driveTrain.dir/build.make DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o.provides.build
.PHONY : DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o.provides

DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o.provides.build: DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o

DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o: DriveTrain/CMakeFiles/driveTrain.dir/flags.make
DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o: /home/roboops/2014Software/groovy2014/src/DriveTrain/src/RobotechMotor.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/roboops/2014Software/groovy2014/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o -c /home/roboops/2014Software/groovy2014/src/DriveTrain/src/RobotechMotor.cpp

DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.i"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/roboops/2014Software/groovy2014/src/DriveTrain/src/RobotechMotor.cpp > CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.i

DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.s"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/roboops/2014Software/groovy2014/src/DriveTrain/src/RobotechMotor.cpp -o CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.s

DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o.requires:
.PHONY : DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o.requires

DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o.provides: DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o.requires
	$(MAKE) -f DriveTrain/CMakeFiles/driveTrain.dir/build.make DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o.provides.build
.PHONY : DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o.provides

DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o.provides.build: DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o

DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o: DriveTrain/CMakeFiles/driveTrain.dir/flags.make
DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o: /home/roboops/2014Software/groovy2014/src/DriveTrain/src/Serial.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/roboops/2014Software/groovy2014/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/driveTrain.dir/src/Serial.cpp.o -c /home/roboops/2014Software/groovy2014/src/DriveTrain/src/Serial.cpp

DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/driveTrain.dir/src/Serial.cpp.i"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/roboops/2014Software/groovy2014/src/DriveTrain/src/Serial.cpp > CMakeFiles/driveTrain.dir/src/Serial.cpp.i

DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/driveTrain.dir/src/Serial.cpp.s"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/roboops/2014Software/groovy2014/src/DriveTrain/src/Serial.cpp -o CMakeFiles/driveTrain.dir/src/Serial.cpp.s

DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o.requires:
.PHONY : DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o.requires

DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o.provides: DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o.requires
	$(MAKE) -f DriveTrain/CMakeFiles/driveTrain.dir/build.make DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o.provides.build
.PHONY : DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o.provides

DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o.provides.build: DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o

# Object files for target driveTrain
driveTrain_OBJECTS = \
"CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o" \
"CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o" \
"CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o" \
"CMakeFiles/driveTrain.dir/src/Serial.cpp.o"

# External object files for target driveTrain
driveTrain_EXTERNAL_OBJECTS =

/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /opt/ros/groovy/lib/libroscpp.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /usr/lib/libboost_signals-mt.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /usr/lib/libboost_filesystem-mt.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /opt/ros/groovy/lib/librosconsole.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /usr/lib/libboost_regex-mt.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /usr/lib/liblog4cxx.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /opt/ros/groovy/lib/libxmlrpcpp.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /opt/ros/groovy/lib/libroscpp_serialization.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /opt/ros/groovy/lib/librostime.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /usr/lib/libboost_date_time-mt.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /usr/lib/libboost_system-mt.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /usr/lib/libboost_thread-mt.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: /opt/ros/groovy/lib/libcpp_common.so
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: DriveTrain/CMakeFiles/driveTrain.dir/build.make
/home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain: DriveTrain/CMakeFiles/driveTrain.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable /home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain"
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/driveTrain.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
DriveTrain/CMakeFiles/driveTrain.dir/build: /home/roboops/2014Software/groovy2014/devel/lib/DriveTrain/driveTrain
.PHONY : DriveTrain/CMakeFiles/driveTrain.dir/build

DriveTrain/CMakeFiles/driveTrain.dir/requires: DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainNode.cpp.o.requires
DriveTrain/CMakeFiles/driveTrain.dir/requires: DriveTrain/CMakeFiles/driveTrain.dir/src/DriveTrainClass.cpp.o.requires
DriveTrain/CMakeFiles/driveTrain.dir/requires: DriveTrain/CMakeFiles/driveTrain.dir/src/RobotechMotor.cpp.o.requires
DriveTrain/CMakeFiles/driveTrain.dir/requires: DriveTrain/CMakeFiles/driveTrain.dir/src/Serial.cpp.o.requires
.PHONY : DriveTrain/CMakeFiles/driveTrain.dir/requires

DriveTrain/CMakeFiles/driveTrain.dir/clean:
	cd /home/roboops/2014Software/groovy2014/build/DriveTrain && $(CMAKE_COMMAND) -P CMakeFiles/driveTrain.dir/cmake_clean.cmake
.PHONY : DriveTrain/CMakeFiles/driveTrain.dir/clean

DriveTrain/CMakeFiles/driveTrain.dir/depend:
	cd /home/roboops/2014Software/groovy2014/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/roboops/2014Software/groovy2014/src /home/roboops/2014Software/groovy2014/src/DriveTrain /home/roboops/2014Software/groovy2014/build /home/roboops/2014Software/groovy2014/build/DriveTrain /home/roboops/2014Software/groovy2014/build/DriveTrain/CMakeFiles/driveTrain.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : DriveTrain/CMakeFiles/driveTrain.dir/depend

