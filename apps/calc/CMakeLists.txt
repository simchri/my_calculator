set(TARGET calc)
my_add_executable(${TARGET})

target_sources(${TARGET} PRIVATE src/main.cpp)

find_package(FTXUI CONFIG REQUIRED)

target_link_libraries(
  ${TARGET}
  PRIVATE view
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component)

install(TARGETS ${TARGET} COMPONENT ${BIN_PACKAGE_COMPONENT_NAME})
