// unit / component under test:
#include <view/view.h>

// rest :
#include <benchmark/benchmark.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;



TEST(IViewTests, onInput_updateLastLineInHistoryWithCurrentResult) {
    auto view = view::ViewModel();
    view.input = "1";
    view::on_new_char_entered(view);
    EXPECT_EQ(view.history.at(0), "1 = 1");
}

TEST(IViewTests, onEnter_newHistoryLine) {
    auto view = view::ViewModel();
    view.input = "1";
    view::on_new_char_entered(view);
    view::on_enter(view);
    EXPECT_EQ(view.history.at(0), "1 = 1");
    EXPECT_EQ(view.history.at(1), "");
}

TEST(IViewTests, existingCalculation_onInput_updateLastHistoryLine) {
    auto view = view::ViewModel();
    view.input = "1";
    view::on_new_char_entered(view);
    view.input = "12";
    view::on_new_char_entered(view);
    EXPECT_EQ(view.history.at(0), "12 = 12");
}

TEST(IViewTests, existingCalculation_onDeleteLastChar_clearLastHistoryLine){
    auto view = view::ViewModel();
    view.input = "1";
    view::on_new_char_entered(view);
    view.input = "";
    view::on_new_char_entered(view);
    EXPECT_EQ(view.history.at(0), "");
}
