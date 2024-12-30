set(TARGET parsing)
my_add_library("${TARGET}" STATIC)


target_sources(${TARGET}
        # add both sources and headers under PRIVATE, even headers that are included by downstream targets
        # public headers are made availble to downstream targets via `target_include_directories`
        # Don't use file globbing. List every file explicitly
        PRIVATE
        include/${TARGET}/parsing.h
        src/parsing.cpp
)

target_include_directories(${TARGET} PUBLIC
        # both should only go to level "include/" so consumers must do #include <my_component/my_component.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/>
)

add_subdirectory(test)
