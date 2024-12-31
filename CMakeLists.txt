cmake_minimum_required(VERSION 3.29)

project(my_calculator)
# project name is provided in the variable CMAKE_PROJECT_NAME for use elsewhere

# configuration (must be done after project() call)
include(cmake/mytarget.cmake)
include(cmake/compiler.cmake)
include(cmake/packaging.cmake)
include(cmake/packaging_tests.cmake)
include(cmake/static_code_analysis.cmake)

# subdirectories containing source code
add_subdirectory(components)
add_subdirectory(apps)
