set(TARGET_UNDER_TEST ${TARGET})
set(TARGET ${TARGET_UNDER_TEST}_unit_tests)
my_add_executable("${TARGET}")

target_sources(${TARGET}
        PRIVATE
        my_first_fixture.cpp
)

find_package(benchmark CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

target_link_libraries(${TARGET}
        PRIVATE
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        benchmark::benchmark
        ${TARGET_UNDER_TEST}
)

add_test(NAME ${TARGET} COMMAND ${TARGET})
set_tests_properties(${TARGET} PROPERTIES LABELS "unit;fast")
