# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/caidingding233/Desktop/jvav/JvavCompiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/caidingding233/Desktop/jvav/JvavCompiler/build

# Include any dependencies generated for this target.
include CMakeFiles/jvav_terminal.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/jvav_terminal.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/jvav_terminal.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/jvav_terminal.dir/flags.make

CMakeFiles/jvav_terminal.dir/codegen:
.PHONY : CMakeFiles/jvav_terminal.dir/codegen

CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.o: CMakeFiles/jvav_terminal.dir/flags.make
CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.o: /Users/caidingding233/Desktop/jvav/JvavCompiler/src/repl/JvavREPL.cpp
CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.o: CMakeFiles/jvav_terminal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/caidingding233/Desktop/jvav/JvavCompiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.o -MF CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.o.d -o CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.o -c /Users/caidingding233/Desktop/jvav/JvavCompiler/src/repl/JvavREPL.cpp

CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/caidingding233/Desktop/jvav/JvavCompiler/src/repl/JvavREPL.cpp > CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.i

CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/caidingding233/Desktop/jvav/JvavCompiler/src/repl/JvavREPL.cpp -o CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.s

CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.o: CMakeFiles/jvav_terminal.dir/flags.make
CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.o: /Users/caidingding233/Desktop/jvav/JvavCompiler/src/repl/jvav_terminal.cpp
CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.o: CMakeFiles/jvav_terminal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/caidingding233/Desktop/jvav/JvavCompiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.o -MF CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.o.d -o CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.o -c /Users/caidingding233/Desktop/jvav/JvavCompiler/src/repl/jvav_terminal.cpp

CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/caidingding233/Desktop/jvav/JvavCompiler/src/repl/jvav_terminal.cpp > CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.i

CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/caidingding233/Desktop/jvav/JvavCompiler/src/repl/jvav_terminal.cpp -o CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.s

CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.o: CMakeFiles/jvav_terminal.dir/flags.make
CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.o: /Users/caidingding233/Desktop/jvav/JvavCompiler/src/JvavCompiler.cpp
CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.o: CMakeFiles/jvav_terminal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/caidingding233/Desktop/jvav/JvavCompiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.o -MF CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.o.d -o CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.o -c /Users/caidingding233/Desktop/jvav/JvavCompiler/src/JvavCompiler.cpp

CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/caidingding233/Desktop/jvav/JvavCompiler/src/JvavCompiler.cpp > CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.i

CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/caidingding233/Desktop/jvav/JvavCompiler/src/JvavCompiler.cpp -o CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.s

CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.o: CMakeFiles/jvav_terminal.dir/flags.make
CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.o: /Users/caidingding233/Desktop/jvav/JvavCompiler/src/codegen/CodeGenerator.cpp
CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.o: CMakeFiles/jvav_terminal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/caidingding233/Desktop/jvav/JvavCompiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.o -MF CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.o.d -o CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.o -c /Users/caidingding233/Desktop/jvav/JvavCompiler/src/codegen/CodeGenerator.cpp

CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/caidingding233/Desktop/jvav/JvavCompiler/src/codegen/CodeGenerator.cpp > CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.i

CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/caidingding233/Desktop/jvav/JvavCompiler/src/codegen/CodeGenerator.cpp -o CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.s

CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.o: CMakeFiles/jvav_terminal.dir/flags.make
CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.o: /Users/caidingding233/Desktop/jvav/JvavCompiler/src/codegen/LLVMCodeGenerator.cpp
CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.o: CMakeFiles/jvav_terminal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/caidingding233/Desktop/jvav/JvavCompiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.o -MF CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.o.d -o CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.o -c /Users/caidingding233/Desktop/jvav/JvavCompiler/src/codegen/LLVMCodeGenerator.cpp

CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/caidingding233/Desktop/jvav/JvavCompiler/src/codegen/LLVMCodeGenerator.cpp > CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.i

CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/caidingding233/Desktop/jvav/JvavCompiler/src/codegen/LLVMCodeGenerator.cpp -o CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.s

CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.o: CMakeFiles/jvav_terminal.dir/flags.make
CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.o: /Users/caidingding233/Desktop/jvav/JvavCompiler/src/lexer/Lexer.cpp
CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.o: CMakeFiles/jvav_terminal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/caidingding233/Desktop/jvav/JvavCompiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.o -MF CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.o.d -o CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.o -c /Users/caidingding233/Desktop/jvav/JvavCompiler/src/lexer/Lexer.cpp

CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/caidingding233/Desktop/jvav/JvavCompiler/src/lexer/Lexer.cpp > CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.i

CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/caidingding233/Desktop/jvav/JvavCompiler/src/lexer/Lexer.cpp -o CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.s

CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.o: CMakeFiles/jvav_terminal.dir/flags.make
CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.o: /Users/caidingding233/Desktop/jvav/JvavCompiler/src/lexer/Token.cpp
CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.o: CMakeFiles/jvav_terminal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/caidingding233/Desktop/jvav/JvavCompiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.o -MF CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.o.d -o CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.o -c /Users/caidingding233/Desktop/jvav/JvavCompiler/src/lexer/Token.cpp

CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/caidingding233/Desktop/jvav/JvavCompiler/src/lexer/Token.cpp > CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.i

CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/caidingding233/Desktop/jvav/JvavCompiler/src/lexer/Token.cpp -o CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.s

CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.o: CMakeFiles/jvav_terminal.dir/flags.make
CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.o: /Users/caidingding233/Desktop/jvav/JvavCompiler/src/optimizer/Optimizer.cpp
CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.o: CMakeFiles/jvav_terminal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/caidingding233/Desktop/jvav/JvavCompiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.o -MF CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.o.d -o CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.o -c /Users/caidingding233/Desktop/jvav/JvavCompiler/src/optimizer/Optimizer.cpp

CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/caidingding233/Desktop/jvav/JvavCompiler/src/optimizer/Optimizer.cpp > CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.i

CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/caidingding233/Desktop/jvav/JvavCompiler/src/optimizer/Optimizer.cpp -o CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.s

CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.o: CMakeFiles/jvav_terminal.dir/flags.make
CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.o: /Users/caidingding233/Desktop/jvav/JvavCompiler/src/parser/Parser.cpp
CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.o: CMakeFiles/jvav_terminal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/caidingding233/Desktop/jvav/JvavCompiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.o -MF CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.o.d -o CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.o -c /Users/caidingding233/Desktop/jvav/JvavCompiler/src/parser/Parser.cpp

CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/caidingding233/Desktop/jvav/JvavCompiler/src/parser/Parser.cpp > CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.i

CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/caidingding233/Desktop/jvav/JvavCompiler/src/parser/Parser.cpp -o CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.s

# Object files for target jvav_terminal
jvav_terminal_OBJECTS = \
"CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.o" \
"CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.o" \
"CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.o" \
"CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.o" \
"CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.o" \
"CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.o" \
"CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.o" \
"CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.o" \
"CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.o"

# External object files for target jvav_terminal
jvav_terminal_EXTERNAL_OBJECTS =

jvav_terminal: CMakeFiles/jvav_terminal.dir/src/repl/JvavREPL.cpp.o
jvav_terminal: CMakeFiles/jvav_terminal.dir/src/repl/jvav_terminal.cpp.o
jvav_terminal: CMakeFiles/jvav_terminal.dir/src/JvavCompiler.cpp.o
jvav_terminal: CMakeFiles/jvav_terminal.dir/src/codegen/CodeGenerator.cpp.o
jvav_terminal: CMakeFiles/jvav_terminal.dir/src/codegen/LLVMCodeGenerator.cpp.o
jvav_terminal: CMakeFiles/jvav_terminal.dir/src/lexer/Lexer.cpp.o
jvav_terminal: CMakeFiles/jvav_terminal.dir/src/lexer/Token.cpp.o
jvav_terminal: CMakeFiles/jvav_terminal.dir/src/optimizer/Optimizer.cpp.o
jvav_terminal: CMakeFiles/jvav_terminal.dir/src/parser/Parser.cpp.o
jvav_terminal: CMakeFiles/jvav_terminal.dir/build.make
jvav_terminal: CMakeFiles/jvav_terminal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/caidingding233/Desktop/jvav/JvavCompiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable jvav_terminal"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jvav_terminal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/jvav_terminal.dir/build: jvav_terminal
.PHONY : CMakeFiles/jvav_terminal.dir/build

CMakeFiles/jvav_terminal.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/jvav_terminal.dir/cmake_clean.cmake
.PHONY : CMakeFiles/jvav_terminal.dir/clean

CMakeFiles/jvav_terminal.dir/depend:
	cd /Users/caidingding233/Desktop/jvav/JvavCompiler/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/caidingding233/Desktop/jvav/JvavCompiler /Users/caidingding233/Desktop/jvav/JvavCompiler /Users/caidingding233/Desktop/jvav/JvavCompiler/build /Users/caidingding233/Desktop/jvav/JvavCompiler/build /Users/caidingding233/Desktop/jvav/JvavCompiler/build/CMakeFiles/jvav_terminal.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/jvav_terminal.dir/depend

