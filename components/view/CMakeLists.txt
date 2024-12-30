set(TARGET view)
my_add_library("${TARGET}" STATIC)


target_sources(${TARGET}
        PRIVATE
        include/${TARGET}/view.h
        src/view.cpp
)

target_link_libraries(${TARGET}
        PRIVATE
        parsing
)

target_include_directories(${TARGET} PUBLIC
        # both should only go to level "include/" so consumers must do #include <my_component/my_component.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/>
)

add_subdirectory(test)
