set(TARGET view)
my_add_library("${TARGET}" STATIC)

target_sources(${TARGET}
        PUBLIC FILE_SET all_my_modules TYPE CXX_MODULES FILES
        view.cppm
)

target_link_libraries(${TARGET}
        PRIVATE
        parsing
)

add_subdirectory(test)
