set(TARGET parsing)
my_add_library("${TARGET}" STATIC)

target_sources(
  ${TARGET}
  # add both sources and headers under PRIVATE, even headers that are included
  # by downstream targets public headers are made availble to downstream targets
  # via `target_include_directories` Don't use file globbing. List every file
  # explicitly
  PUBLIC FILE_SET all_my_modules TYPE CXX_MODULES FILES
  parsing.cppm
  )

add_subdirectory(test)
