message(STATUS "PACKAGING tests setup ...")
enable_testing()

add_test(
  NAME check_package_contents
  COMMAND
    ${CMAKE_SOURCE_DIR}/scripts/package_contents.bash
    ${CMAKE_CURRENT_BINARY_DIR}             # expect to find in this directory ..
    "my_calculator"                         # a .deb package (file) with package name "my-calc"
    "/usr/bin/calc"                         # containing the file /usr/bin/calc
)
set_tests_properties(check_package_contents PROPERTIES LABELS "packaging;slow")
