// unit / component under test:
#include <parsing/parsing.h>

// rest :
#include <benchmark/benchmark.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace parsing;
using namespace testing;

static void BM_eval(benchmark::State& state) {
    for (auto _ : state) {
        eval("1+2*3-7+4+1234*54-999*45+2/2");
        eval("1+2/2");
    }
}
// Register the function as a benchmark
BENCHMARK(BM_eval);

BENCHMARK_MAIN();
