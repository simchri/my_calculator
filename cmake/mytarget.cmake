macro(_check_deact _target_name)

  set(_cmake_project_name ${CMAKE_PROJECT_NAME})
  string(TOUPPER ${_cmake_project_name} _cmake_project_name_upper)
  string(REPLACE "-" "_" _cmake_project_name_upper ${_cmake_project_name_upper})

  string(TOUPPER ${_target_name} _target_name_upper)

  set(_deactivation_variable_name  ${_cmake_project_name_upper}_COMPONENT_${_target_name_upper} )
  set(${_deactivation_variable_name}  ON  CACHE   BOOL    "Enable ${_target_name_upper}")

  if(NOT ${_deactivation_variable_name})
      message(STATUS "SKIP target ${_target_name}")
      return()
  endif()

endmacro()


macro(_cleanup)
  unset(_target_name)
  unset(_lib_type)
  unset(_cmake_project_name)
  unset(_cmake_project_name_upper)
  unset(_target_name_upper)
  unset(_deactivation_variable_name)
endmacro()


macro(my_add_library _target_name _lib_type)

  _check_deact(${_target_name})

  message(STATUS "LIB ${_target_name} - exclude from config with `-D ${_cmake_project_name_upper}_COMPONENT_${_target_name_upper}=OFF`")

  message(STATUS "... configure ${_lib_type} lib target           ${_target_name}")
  add_library(${_target_name} ${_lib_type} ${ARGN})

  _cleanup()

endmacro()

macro(my_add_executable _target_name)

  _check_deact(${_target_name})
  string(REGEX MATCH "test(s)?$" _test_target ${_target_name})

  if(_test_target)
    message(STATUS "... configure TEST (executable) target    ${_target_name}")
  else()
    message(STATUS "APP ${_target_name} - exclude from config with `-D ${_cmake_project_name_upper}_COMPONENT_${_target_name_upper}=OFF`")
    message(STATUS "... configure executable target           ${_target_name}")
  endif()

  add_executable(${_target_name} ${ARGN})

  _cleanup()

endmacro()
