// unit / component under test:

// rest :
#include <gmock/gmock.h>
#include <gtest/gtest.h>

import parsing;

using namespace testing;
using namespace parsing;

void EXPECT_VECU_EQ(const std::vector<std::unique_ptr<node>>& x, const std::vector<std::unique_ptr<node>>& y) {

    // note that if this assert fails, it only returns from this function, but does not stop the rest of the test
    ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";

    for (size_t i = 0; i < x.size(); ++i) {
        EXPECT_EQ(*x[i], *y[i]) << "Vectors x and y differ at index " << i;
    }
}

// AST node comparison tests:
TEST(IASTNodeUnitTests, input_1_1_eq) {
    auto left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto right = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    EXPECT_EQ(*left, *right);
}

TEST(IASTNodeUnitTests, input_1_2_ne) {
    auto left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto right = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 2});
    EXPECT_NE(*left, *right);
}

TEST(IASTNodeUnitTests, input_1_1withChild_ne) {
    auto left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto right = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    right->left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    EXPECT_NE(*left, *right);
}

TEST(IASTNodeUnitTests, input_1withChild_1withChild_eq) {
    auto left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto right = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    left->left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    right->left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    EXPECT_EQ(*left, *right);
}

TEST(IASTNodeUnitTests, input_1withChild_1withChildOtherValue_ne) {
    auto left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto right = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    left->left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    right->left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 2});
    EXPECT_NE(*left, *right);
}


TEST(ITokenizerUnitTests, tokenize_1) {
    auto expected = std::vector<std::unique_ptr<node>>{};
    expected.push_back(std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1}));
    EXPECT_VECU_EQ(expected, {tokenize("1")});
}

TEST(ITokenizerUnitTests, tokenize_2plus) {
    auto expected = std::vector<std::unique_ptr<node>>{};
    expected.push_back(std::make_unique<node>(node{.type = NUM_LITERAL, .value = 2}));
    expected.push_back(std::make_unique<node>(node{.type = OPERATOR_PLUS}));
    EXPECT_VECU_EQ(expected, tokenize("2+"));
}

TEST(ITokenizerUnitTests, tokenize_2plus1) {
    auto expected = std::vector<std::unique_ptr<node>>{};
    expected.push_back(std::make_unique<node>(node{.type = NUM_LITERAL, .value = 2}));
    expected.push_back(std::make_unique<node>(node{.type = OPERATOR_PLUS}));
    expected.push_back(std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1}));
    EXPECT_VECU_EQ(expected, tokenize("2+1"));
}

TEST(ITokenizerUnitTests, tokenize_10) {
    auto expected = std::vector<std::unique_ptr<node>>{};
    expected.push_back(std::make_unique<node>(node{.type = NUM_LITERAL, .value = 10}));
    EXPECT_VECU_EQ(expected, tokenize("10"));
}

TEST(ITokenizerUnitTests, tokenize_10plus2) {
    auto expected = std::vector<std::unique_ptr<node>>{};
    expected.push_back(std::make_unique<node>(node{.type = NUM_LITERAL, .value = 10}));
    expected.push_back(std::make_unique<node>(node{.type = OPERATOR_PLUS}));
    expected.push_back(std::make_unique<node>(node{.type = NUM_LITERAL, .value = 2}));
    EXPECT_VECU_EQ(expected, tokenize("10+2"));
}

TEST(ITokenizerUnitTests, tokenize_10plus2plus12) {
    auto expected = std::vector<std::unique_ptr<node>>{};
    expected.push_back(std::make_unique<node>(node{.type = NUM_LITERAL, .value = 10}));
    expected.push_back(std::make_unique<node>(node{.type = OPERATOR_PLUS}));
    expected.push_back(std::make_unique<node>(node{.type = NUM_LITERAL, .value = 2}));
    expected.push_back(std::make_unique<node>(node{.type = OPERATOR_PLUS}));
    expected.push_back(std::make_unique<node>(node{.type = NUM_LITERAL, .value = 12}));
    EXPECT_VECU_EQ(expected, tokenize("10+2+12"));
}

// -
TEST(ITokenizerUnitTests, tokenize_minus) {
    auto expected = std::vector<std::unique_ptr<node>>{};
    expected.push_back(std::make_unique<node>(node{.type = OPERATOR_MINUS}));
    EXPECT_VECU_EQ(expected, tokenize("-"));
}

TEST(ITokenizerUnitTests, tokenize_minus1) {
    auto expected = std::vector<std::unique_ptr<node>>{};
    expected.push_back(std::make_unique<node>(node{.type = OPERATOR_MINUS}));
    expected.push_back(std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1}));
    EXPECT_VECU_EQ(expected, tokenize("-1"));
}

TEST(ITokenizerUnitTests, tokenize_multiply) {
    auto expected = std::vector<std::unique_ptr<node>>{};
    expected.push_back(std::make_unique<node>(node{.type = OPERATOR_MULTIPLY}));
    EXPECT_VECU_EQ(expected, tokenize("*"));
}

TEST(ITokenizerUnitTests, tokenize_divide) {
    auto expected = std::vector<std::unique_ptr<node>>{};
    expected.push_back(std::make_unique<node>(node{.type = OPERATOR_DIVIDE}));
    EXPECT_VECU_EQ(expected, tokenize("/"));
}

// parsing tests - error cases


TEST(IParserUnitTests, parse_plus_1) {
    EXPECT_THROW(parse("+1"), std::invalid_argument);
}

TEST(IParserUnitTests, parse_plus) {
    EXPECT_THROW(parse("+"), std::invalid_argument);
}

TEST(IParserUnitTests, parse_1_plus) {
    EXPECT_THROW(parse("1+"), std::invalid_argument);
}

TEST(IParserUnitTests, parse_1_plus_1_plus) {
    EXPECT_THROW(parse("1+1+"), std::invalid_argument);
}

TEST(IParserUnitTests, parse_invalidInput_times1) {
    EXPECT_THROW(parse("*1"), std::invalid_argument);
}

// sample on how to test for contents of error message (not yet needed)
TEST(IParserUnitTests, parse_1_times) {
    EXPECT_THAT(
            []() {
                parse("1*");
            },
            ThrowsMessage<std::invalid_argument>(HasSubstr("error")));
}

TEST(IParserUnitTests, parse_invalidInput_1plus1overTimes) {
    EXPECT_THROW(parse("1+1/*"), std::invalid_argument);
}

TEST(IParserUnitTests, parse_invalidInput_1overTimes) {
    EXPECT_THROW(parse("1/*"), std::invalid_argument);
}

TEST(IParserUnitTests, parse_invalidInput_1overTimesPlus1) {
    EXPECT_THROW(parse("1/*+1"), std::invalid_argument);
}

TEST(IParserUnitTests, parse_invalidInput_Misc) {
    EXPECT_THROW(parse("**"), std::invalid_argument);
    EXPECT_THROW(parse("*"), std::invalid_argument);
    EXPECT_THROW(parse("+*+"), std::invalid_argument);
    EXPECT_THROW(parse("*+*"), std::invalid_argument);
    EXPECT_THROW(parse("//++"), std::invalid_argument);
    EXPECT_THROW(parse("2*+*1"), std::invalid_argument);
    EXPECT_THROW(parse("1+4/3+234+4+5+6+/3*34"), std::invalid_argument);
    EXPECT_THROW(parse("1+*1"), std::invalid_argument);
    EXPECT_THROW(parse("1+*1-"), std::invalid_argument);
}

// Parsing - happy path
TEST(IParserUnitTests, parse_1) {
    auto result = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    EXPECT_EQ(*parse("1"), *result);
}

TEST(IParserUnitTests, parse_2) {
    auto result = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 2});
    EXPECT_EQ(*parse("2"), *result.get());
}

TEST(IParserUnitTests, parse_1_plus_1) {
    auto left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto right = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto result = std::make_unique<node>(node{.type = OPERATOR_PLUS, .left = std::move(left), .right = std::move(right)});
    EXPECT_EQ(*parse("1+1"), *result);
}

TEST(IParserUnitTests, parse_1_plus_1_plus_1) {
    auto left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto right = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto center = std::make_unique<node>(node{.type = OPERATOR_PLUS, .left = std::move(left), .right = std::move(right)});
    auto right2 = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto result = std::make_unique<node>(node{.type = OPERATOR_PLUS, .left = std::move(center), .right = std::move(right2)});
    EXPECT_EQ(*parse("1+1+1"), *result);
}

TEST(IParserUnitTests, parse_1_minus_1) {
    auto left = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto right = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto result = std::make_unique<node>(node{.type = OPERATOR_MINUS, .left = std::move(left), .right = std::move(right)});
    EXPECT_EQ(*parse("1-1"), *result);
}

TEST(IParserUnitTests, parse_1_plus_2_times_4) {
    auto one = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 1});
    auto two = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 2});
    auto four = std::make_unique<node>(node{.type = NUM_LITERAL, .value = 4});
    auto times = std::make_unique<node>(node{.type = OPERATOR_MULTIPLY, .left = std::move(two), .right = std::move(four)});
    auto plus = std::make_unique<node>(node{.type = OPERATOR_PLUS, .left = std::move(one), .right = std::move(times)});
    EXPECT_EQ(*parse("1+2*4"), *plus);
}

// Evaluation
TEST(IEvaluationUnitTests, eval_1) {
    EXPECT_EQ(1, eval("1"));
}

TEST(IEvaluationUnitTests, eval_1_plus_1) {
    EXPECT_EQ(2, eval("1+1"));
}

TEST(IEvaluationUnitTests, eval_1_plus_1_plus_1) {
    EXPECT_EQ(3, eval("1+1+1"));
}

TEST(IEvaluationUnitTests, eval_1_minus_1) {
    EXPECT_EQ(0, eval("1-1"));
}

TEST(IEvaluationUnitTests, eval_summation_and_subtraction) {
    EXPECT_EQ(2, eval("1+5-4"));
    EXPECT_EQ(2, eval("1-4+5"));
    EXPECT_EQ(0, eval("1-4+5+8+90-100"));
}

TEST(IEvaluationUnitTests, eval_multiplication_precedence) {
    EXPECT_EQ(2, eval("1*2"));
    EXPECT_EQ(7, eval("1+3*2"));
    EXPECT_EQ(-1, eval("1+3*2-4-2*2"));
}

TEST(IEvaluationUnitTests, eval_multiplication_precedenceThreeMultiplicators) {
    EXPECT_EQ(1, eval("1*1*1"));
    EXPECT_EQ(2, eval("1+1*1"));
    EXPECT_EQ(2, eval("1*1+1"));
    EXPECT_EQ(2, eval("1*1+1/1"));
    EXPECT_EQ(2, eval("1*1+1/1*1/1*1"));
    EXPECT_EQ(2, eval("1*1+1/2*2/4.5*4.5"));
}

TEST(IEvaluationUnitTests, eval_division_integer) {
    EXPECT_EQ(1, eval("2/2"));
    EXPECT_EQ(2, eval("4/2"));
    EXPECT_EQ(-2, eval("1+4/2-5"));
}

TEST(IEvaluationUnitTests, eval_division_floatResult) {
    EXPECT_EQ(0.5, eval("1/2"));
}

TEST(IEvaluationUnitTests, eval_division_floatInput) {
    EXPECT_EQ(1.5, eval("1+0.5"));
    EXPECT_EQ(1.5, eval("1.5/100*100"));
}

// parenthesis

TEST(IEvaluationUnitTests, eval_OP_throws) {
    EXPECT_THROW(eval("("), std::invalid_argument);
}

TEST(IEvaluationUnitTests, eval_CP_throws) {
    EXPECT_THROW(eval(")"), std::invalid_argument);
}

TEST(IEvaluationUnitTests, eval_emptyParenthesisPair_throws) {
    EXPECT_THROW(eval("()"), std::invalid_argument);
}

TEST(IEvaluationUnitTests, eval_1_in_parenthesis) {
    EXPECT_EQ(1, eval("(1)"));
}

TEST(IEvaluationUnitTests, eval_1_plus_1_in_parenthesis) {
    EXPECT_EQ(2, eval("(1+1)"));
}

TEST(IEvaluationUnitTests, eval_1_plus_1_in_paras_plus_1) {
    EXPECT_EQ(3, eval("(1+1)+1"));
}

// SFI: better test names
TEST(IEvaluationUnitTests, eval_1_plus_1_plus_1_in_paras) {
    EXPECT_EQ(3, eval("1+(1+1)"));
}

TEST(IEvaluationUnitTests, eval_multiParenthesisPairs) {
    EXPECT_EQ(5, eval("1+(1+1)+(1+1)"));
}

TEST(IEvaluationUnitTests, eval_nestedParenthesis) {
    EXPECT_EQ(4, eval("1+(1+(1+1))"));
}

TEST(IEvaluationUnitTests, eval_multiplePairsOneUnbalanced_Throws) {
    EXPECT_THROW(eval("(1+1)+(1"), std::invalid_argument);
    EXPECT_THROW(eval("(1+1+(1+1)"), std::invalid_argument);
}

TEST(IEvaluationUnitTests, eval_parenthesisMultiplicationPrecedence) {
    EXPECT_EQ(4, eval("(1+1)*2"));
    EXPECT_EQ(4, eval("2*(1+1)"));
}

TEST(IEvaluationUnitTests, eval_multiplePrenthesis) {
    EXPECT_EQ(3, eval("(1*3)"));
    EXPECT_EQ(7, eval("2*(1+1)+(1*3)"));
}

// multipl. + nested parenthesis
TEST(IEvaluationUnitTests, eval_nestedParenthesisProduct) {
    EXPECT_EQ(8, eval("2*(2*(1+1))"));
    EXPECT_EQ(8, eval("2*(2+(1+1))"));
    EXPECT_EQ(6, eval("2+(2*(1+1))"));

    EXPECT_EQ(6, eval("2+((1+1)*2)"));
}

TEST(IEvaluationUnitTests, eval_ParenthesisAsSecondProductTerm) {
    EXPECT_EQ(12, eval("2*(1+2+3)"));
    EXPECT_EQ(14, eval("2+2*(1+2+3)"));
    EXPECT_EQ(26, eval("2+((1+1)*2*(1+2+3))"));
    EXPECT_EQ(11, eval("2+(1+1)*2*1+2+3"));
}

// parenthesis & floats
TEST(IEvaluationUnitTests, eval_parenthesisAndFloats) {
    EXPECT_EQ(2.8, eval("2*(1+0.4)"));
    EXPECT_EQ(4, eval("2+(1+1)*2*0.5"));
    EXPECT_EQ(1.3, eval("(1.1+0.2)*1.0"));
}

TEST(IEvaluationUnitTests, eval_unbalancedParenthesis_Throw){
    EXPECT_THROW(eval("2+2*(1+2+3))"), std::invalid_argument);
}

TEST(IEvaluationUnitTests, eval_noClosingPT_throw) {
    EXPECT_THROW(eval("(1"), std::invalid_argument);
}

TEST(IEvaluationUnitTests, eval_1_times_1_in_paras) {
    EXPECT_EQ(1, eval("1*(1)"));
}
