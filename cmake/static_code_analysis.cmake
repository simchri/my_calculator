message(STATUS "Static code analysis tests setup ...")
enable_testing()

add_test(
  NAME cppcheck
  COMMAND cppcheck --error-exitcode=1 ${CMAKE_SOURCE_DIR}/apps ${CMAKE_SOURCE_DIR}/components -i ${CMAKE_SOURCE_DIR}/apps/ftxexamples
)
set_tests_properties(cppcheck PROPERTIES LABELS "sca;slow")
