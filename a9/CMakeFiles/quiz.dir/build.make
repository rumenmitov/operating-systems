# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rumen/University/semester_3/os/assignments/a9

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rumen/University/semester_3/os/assignments/a9

# Include any dependencies generated for this target.
include CMakeFiles/quiz.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/quiz.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/quiz.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/quiz.dir/flags.make

CMakeFiles/quiz.dir/src/quiz.c.o: CMakeFiles/quiz.dir/flags.make
CMakeFiles/quiz.dir/src/quiz.c.o: src/quiz.c
CMakeFiles/quiz.dir/src/quiz.c.o: CMakeFiles/quiz.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/rumen/University/semester_3/os/assignments/a9/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/quiz.dir/src/quiz.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/quiz.dir/src/quiz.c.o -MF CMakeFiles/quiz.dir/src/quiz.c.o.d -o CMakeFiles/quiz.dir/src/quiz.c.o -c /home/rumen/University/semester_3/os/assignments/a9/src/quiz.c

CMakeFiles/quiz.dir/src/quiz.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/quiz.dir/src/quiz.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rumen/University/semester_3/os/assignments/a9/src/quiz.c > CMakeFiles/quiz.dir/src/quiz.c.i

CMakeFiles/quiz.dir/src/quiz.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/quiz.dir/src/quiz.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rumen/University/semester_3/os/assignments/a9/src/quiz.c -o CMakeFiles/quiz.dir/src/quiz.c.s

CMakeFiles/quiz.dir/src/fetch.c.o: CMakeFiles/quiz.dir/flags.make
CMakeFiles/quiz.dir/src/fetch.c.o: src/fetch.c
CMakeFiles/quiz.dir/src/fetch.c.o: CMakeFiles/quiz.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/rumen/University/semester_3/os/assignments/a9/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/quiz.dir/src/fetch.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/quiz.dir/src/fetch.c.o -MF CMakeFiles/quiz.dir/src/fetch.c.o.d -o CMakeFiles/quiz.dir/src/fetch.c.o -c /home/rumen/University/semester_3/os/assignments/a9/src/fetch.c

CMakeFiles/quiz.dir/src/fetch.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/quiz.dir/src/fetch.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rumen/University/semester_3/os/assignments/a9/src/fetch.c > CMakeFiles/quiz.dir/src/fetch.c.i

CMakeFiles/quiz.dir/src/fetch.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/quiz.dir/src/fetch.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rumen/University/semester_3/os/assignments/a9/src/fetch.c -o CMakeFiles/quiz.dir/src/fetch.c.s

CMakeFiles/quiz.dir/src/parse.c.o: CMakeFiles/quiz.dir/flags.make
CMakeFiles/quiz.dir/src/parse.c.o: src/parse.c
CMakeFiles/quiz.dir/src/parse.c.o: CMakeFiles/quiz.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/rumen/University/semester_3/os/assignments/a9/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/quiz.dir/src/parse.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/quiz.dir/src/parse.c.o -MF CMakeFiles/quiz.dir/src/parse.c.o.d -o CMakeFiles/quiz.dir/src/parse.c.o -c /home/rumen/University/semester_3/os/assignments/a9/src/parse.c

CMakeFiles/quiz.dir/src/parse.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/quiz.dir/src/parse.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rumen/University/semester_3/os/assignments/a9/src/parse.c > CMakeFiles/quiz.dir/src/parse.c.i

CMakeFiles/quiz.dir/src/parse.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/quiz.dir/src/parse.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rumen/University/semester_3/os/assignments/a9/src/parse.c -o CMakeFiles/quiz.dir/src/parse.c.s

CMakeFiles/quiz.dir/src/play.c.o: CMakeFiles/quiz.dir/flags.make
CMakeFiles/quiz.dir/src/play.c.o: src/play.c
CMakeFiles/quiz.dir/src/play.c.o: CMakeFiles/quiz.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/rumen/University/semester_3/os/assignments/a9/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/quiz.dir/src/play.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/quiz.dir/src/play.c.o -MF CMakeFiles/quiz.dir/src/play.c.o.d -o CMakeFiles/quiz.dir/src/play.c.o -c /home/rumen/University/semester_3/os/assignments/a9/src/play.c

CMakeFiles/quiz.dir/src/play.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/quiz.dir/src/play.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rumen/University/semester_3/os/assignments/a9/src/play.c > CMakeFiles/quiz.dir/src/play.c.i

CMakeFiles/quiz.dir/src/play.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/quiz.dir/src/play.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rumen/University/semester_3/os/assignments/a9/src/play.c -o CMakeFiles/quiz.dir/src/play.c.s

CMakeFiles/quiz.dir/src/errors.c.o: CMakeFiles/quiz.dir/flags.make
CMakeFiles/quiz.dir/src/errors.c.o: src/errors.c
CMakeFiles/quiz.dir/src/errors.c.o: CMakeFiles/quiz.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/rumen/University/semester_3/os/assignments/a9/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/quiz.dir/src/errors.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/quiz.dir/src/errors.c.o -MF CMakeFiles/quiz.dir/src/errors.c.o.d -o CMakeFiles/quiz.dir/src/errors.c.o -c /home/rumen/University/semester_3/os/assignments/a9/src/errors.c

CMakeFiles/quiz.dir/src/errors.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/quiz.dir/src/errors.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rumen/University/semester_3/os/assignments/a9/src/errors.c > CMakeFiles/quiz.dir/src/errors.c.i

CMakeFiles/quiz.dir/src/errors.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/quiz.dir/src/errors.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rumen/University/semester_3/os/assignments/a9/src/errors.c -o CMakeFiles/quiz.dir/src/errors.c.s

# Object files for target quiz
quiz_OBJECTS = \
"CMakeFiles/quiz.dir/src/quiz.c.o" \
"CMakeFiles/quiz.dir/src/fetch.c.o" \
"CMakeFiles/quiz.dir/src/parse.c.o" \
"CMakeFiles/quiz.dir/src/play.c.o" \
"CMakeFiles/quiz.dir/src/errors.c.o"

# External object files for target quiz
quiz_EXTERNAL_OBJECTS =

quiz: CMakeFiles/quiz.dir/src/quiz.c.o
quiz: CMakeFiles/quiz.dir/src/fetch.c.o
quiz: CMakeFiles/quiz.dir/src/parse.c.o
quiz: CMakeFiles/quiz.dir/src/play.c.o
quiz: CMakeFiles/quiz.dir/src/errors.c.o
quiz: CMakeFiles/quiz.dir/build.make
quiz: CMakeFiles/quiz.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/rumen/University/semester_3/os/assignments/a9/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable quiz"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/quiz.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/quiz.dir/build: quiz
.PHONY : CMakeFiles/quiz.dir/build

CMakeFiles/quiz.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/quiz.dir/cmake_clean.cmake
.PHONY : CMakeFiles/quiz.dir/clean

CMakeFiles/quiz.dir/depend:
	cd /home/rumen/University/semester_3/os/assignments/a9 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rumen/University/semester_3/os/assignments/a9 /home/rumen/University/semester_3/os/assignments/a9 /home/rumen/University/semester_3/os/assignments/a9 /home/rumen/University/semester_3/os/assignments/a9 /home/rumen/University/semester_3/os/assignments/a9/CMakeFiles/quiz.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/quiz.dir/depend

