set(compile_warning_options "")
string(APPEND compile_warning_options "-Werror" ) # treat warnings as errors
string(APPEND compile_warning_options " -Wall" ) # enable all warnings for GCC and Clang
string(APPEND compile_warning_options " -Wextra" ) # enable some more wanings not enabled by -Wall
string(APPEND compile_warning_options " -Wunused" ) # enable warnings for unused variables
string(APPEND compile_warning_options " -Wformat" ) # enable warnings for format strings
string(APPEND compile_warning_options " -Wmissing-include-dirs" ) # enable warnings for missing include directories
string(APPEND compile_warning_options " -Wno-pessimizing-move" ) # disable warnings for pessimizing moves
string(APPEND compile_warning_options " -Wfloat-equal" ) # enable warnings for floating point comparisons
string(APPEND compile_warning_options " -Wswitch-default" ) # enable warnings for missing default cases in switch statements
string(APPEND compile_warning_options " -Wdouble-promotion" ) # enable warnings for implicit double to float promotions
string(APPEND compile_warning_options " -Wswitch-enum" ) # enable warnings for switch statements with enums
string(APPEND compile_warning_options " -Wpedantic" ) # enable warnings for pedantic errors
string(APPEND compile_warning_options " -pedantic-errors" ) # enables strict standard conformance

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${compile_warning_options}")

