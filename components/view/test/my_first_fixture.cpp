// unit / component under test:
#include <view/view.h>

// rest :
#include <benchmark/benchmark.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;


TEST(IViewTests, onEnterEvaluateAndAddToHistory) {
    auto view = view::ViewModel();
    view.input = "1+1";
    view::on_new_char_entered(view);
    view::on_enter(view);
    EXPECT_EQ(view.history.at(0), "1+1 = 2");
}
