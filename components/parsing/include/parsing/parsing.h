#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace parsing {

    enum Type { UNDEFINED, NUM_LITERAL, OPERATOR_PLUS, OPERATOR_MINUS, OPERATOR_MULTIPLY, OPERATOR_DIVIDE };

    struct node {
        Type type{UNDEFINED};
        int value{0};
        std::unique_ptr<node> left{};
        std::unique_ptr<node> right{};

        bool operator==(const node& other) const;
    };

    std::vector<std::unique_ptr<node>> tokenize(const std::string& input);
    std::unique_ptr<node> parse(const std::string& input);
    int eval(const std::string& input);

} // namespace parsing
