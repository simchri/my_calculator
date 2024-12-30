set(TARGET_UNDER_TEST ${TARGET})
set(TARGET ${TARGET_UNDER_TEST}_unit_tests)
my_add_executable("${TARGET}")

target_sources(${TARGET}
        PRIVATE
        my_first_fixture.cpp
)

find_package(GTest CONFIG REQUIRED)

target_link_libraries(${TARGET}
        PRIVATE
        GTest::gtest
        GTest::gtest_main
        GTest::gmock

        ${TARGET_UNDER_TEST}
)

add_test(NAME ${TARGET} COMMAND ${TARGET})
set_tests_properties(${TARGET} PROPERTIES LABELS "unit;fast")


# ------------

find_package(benchmark CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

set(TARGET ${TARGET_UNDER_TEST}_benchmarks)
my_add_executable("${TARGET}")

target_sources(${TARGET}
        PRIVATE
        benchmarks.cpp
)

target_link_libraries(${TARGET}
        PRIVATE
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        benchmark::benchmark

        ${TARGET_UNDER_TEST}
)

set(TESTNAME ${TARGET})
add_test(NAME ${TESTNAME} COMMAND ${TARGET} --benchmark_repetitions=10 --benchmark_format=json --benchmark_out=contender.json)
set_tests_properties(${TESTNAME} PROPERTIES LABELS "perf;slow")

set(TESTNAME ${TARGET}_backstop)
add_test(NAME ${TESTNAME} COMMAND ${CMAKE_SOURCE_DIR}/scripts/benchmark_backstop.bash ${CMAKE_CURRENT_SOURCE_DIR}/baseline.json contender.json)
set_tests_properties(${TESTNAME} PROPERTIES LABELS "perf;slow")
set_tests_properties(${TESTNAME} PROPERTIES DEPENDS "{TARGET}")
