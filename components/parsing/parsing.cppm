module;
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <ranges>
#include <vector>

export module parsing;


namespace parsing {
    export typedef double my_float_t;

    export enum Type {
        UNDEFINED,
        NUM_LITERAL,
        OPERATOR_PLUS,
        OPERATOR_MINUS,
        OPERATOR_MULTIPLY,
        OPERATOR_DIVIDE,
        PARENTHESIS_OPEN,
        PARENTHESIS_CLOSE
    };

    export struct node {
        Type type{UNDEFINED};
        double value{0.0};
        std::unique_ptr<node> left{};
        std::unique_ptr<node> right{};

        bool operator==(const node& other) const;
    };


    bool nearly_equal(double a, double b) {
        return std::nextafter(a, std::numeric_limits<double>::lowest()) <= b && std::nextafter(a, std::numeric_limits<double>::max()) >= b;
    }


    bool node::operator==(const node& other) const {
        if ((left == nullptr) != (other.left == nullptr) || (right == nullptr) != (other.right == nullptr)) {
            return false;
        }

        if (type != other.type) {
            return false;
        }

        if (left && *left != *other.left) {
            return false;
        }

        if (right && *right != *other.right) {
            return false;
        }

        if (!nearly_equal(value, other.value)) {
            return false;
        }

        return true;
    }

    bool part_of_numeric_literal(char c) {
        return std::isdigit(c) || c == '.';
    }

    export std::vector<std::unique_ptr<node>> tokenize(const std::string& input) {

        enum LexerState { LITERAL, OPERATOR };

        std::string literal;

        LexerState state;

        std::vector<std::unique_ptr<node>> tokens;

        size_t str_ind = 0;
        size_t str_ind_section_start = 0;
        auto str_ind_end = input.length() - 1;
        while (str_ind <= str_ind_end) {

            if (!part_of_numeric_literal(input[str_ind])) {
                state = OPERATOR;
            } else {
                state = LITERAL;
            }

            if (state == LITERAL) {

                if (str_ind >= str_ind_end || !part_of_numeric_literal(input.at(str_ind + 1))) {

                    literal = input.substr(str_ind_section_start, str_ind - str_ind_section_start + 1);
                    auto new_token = std::make_unique<node>(node{.type = NUM_LITERAL, .value = std::stod(literal)});
                    tokens.push_back(std::move(new_token));
                }

            } else if (state == OPERATOR) {


                if (input[str_ind] == '+') {
                    auto new_token = std::make_unique<node>(node{.type = OPERATOR_PLUS});
                    tokens.push_back(std::move(new_token));
                } else if (input[str_ind] == '-') {
                    auto new_token = std::make_unique<node>(node{.type = OPERATOR_MINUS});
                    tokens.push_back(std::move(new_token));
                } else if (input[str_ind] == '*') {
                    auto new_token = std::make_unique<node>(node{.type = OPERATOR_MULTIPLY});
                    tokens.push_back(std::move(new_token));
                } else if (input[str_ind] == '/') {
                    auto new_token = std::make_unique<node>(node{.type = OPERATOR_DIVIDE});
                    tokens.push_back(std::move(new_token));
                } else if (input[str_ind] == '(') {
                    auto new_token = std::make_unique<node>(node{.type = PARENTHESIS_OPEN});
                    tokens.push_back(std::move(new_token));
                } else if (input[str_ind] == ')') {
                    auto new_token = std::make_unique<node>(node{.type = PARENTHESIS_CLOSE});
                    tokens.push_back(std::move(new_token));
                } else {
                    throw std::invalid_argument("unknown operator");
                }

                str_ind_section_start = str_ind + 1;
            }

            str_ind++;
        }
        return tokens;
    }

    std::unique_ptr<node> pop_back(std::vector<std::unique_ptr<node>>& stack) {

        auto item = std::move(stack.back());
        stack.pop_back();
        return item;
    }

    void collapse_group(std::vector<std::unique_ptr<node>>& stack) {

        auto right = pop_back(stack);
        auto op = pop_back(stack);
        auto left = pop_back(stack);

        op->left = std::move(left);
        op->right = std::move(right);
        stack.push_back(std::move(op));
    }

    bool is_balanced(const std::unique_ptr<node>& root) {
        if (root->type == NUM_LITERAL) {
            return true;
        } else if (root->type == OPERATOR_PLUS || root->type == OPERATOR_MINUS) {
            return root->left && root->right;
        } else if (root->type == OPERATOR_MULTIPLY || root->type == OPERATOR_DIVIDE) {
            return root->left && root->right;
        } else {
            throw std::invalid_argument("error: unknown node type");
        }
    }

    void cut_out(std::size_t from, std::size_t to, std::vector<std::unique_ptr<node>>& input, std::vector<std::unique_ptr<node>>& subset) {
        for (std::size_t k = from; k <= to; k++) {
            subset.push_back(std::move(input[k]));
        }

        auto i_it = input.begin();
        std::advance(i_it, from);

        auto j_it = input.begin();
        std::advance(j_it, to);

        // debugging: print subset

        // std::cout << "subset:" << std::endl;
        // for (auto& item : subset) {
        //     std::cout << item->type << " ";
        // }
        // std::cout << std::endl;

        input.erase(i_it, j_it);
    }

    /**
     * Given an input list of tokens, where the position of an opening parenthesis is known, cut out the parenthesis section from the list
     * and place it in a new list, drop the parenthesis tokens at the edges of the new list.
     *
     * Example: input: tokens: 1+(2+3)*4, i: 3
     *         output: tokens: 1++*4 subset: 2+3
     *
     * @param open_brace_pos position of the opening parenthesis
     * @param tokens list of tokens
     * @param subset list of tokens inside the parenthesis
     */
    void cut_out_parenthesis(std::size_t open_brace_pos,
                             std::vector<std::unique_ptr<node>>& tokens,
                             std::vector<std::unique_ptr<node>>& subset) {

        // find position of matching closing parenthesis in stack
        uint parenthesis_level = 1;
        auto j = open_brace_pos + 1;
        bool found_matching_parenthesis = false;
        for (; j < tokens.size(); ++j) {

            if (tokens[j]->type == PARENTHESIS_OPEN) {
                parenthesis_level += 1;
            }

            if (tokens[j]->type == PARENTHESIS_CLOSE) {
                parenthesis_level -= 1;
            }

            if (parenthesis_level == 0) {
                found_matching_parenthesis = true;
                break;
            }
        }

        if (!found_matching_parenthesis) {
            throw std::invalid_argument("error: No matching closing parenthesis found");
        }

        subset.reserve(j - open_brace_pos);

        cut_out(open_brace_pos, j, tokens, subset);

        subset.erase(subset.begin());
        subset.erase(subset.end());
    }

    std::unique_ptr<node> parse_inner(std::vector<std::unique_ptr<node>>& tokens) {

        if (tokens.size() == 1) {
            if (!is_balanced(tokens[0])) {
                throw std::invalid_argument("error: Invalid syntax");
            }
            return std::move(tokens[0]);
        }

        std::vector<std::unique_ptr<node>> stack;
        stack.reserve(tokens.size());


        for (std::size_t i = 0; i < tokens.size(); ++i) {

            auto& token = tokens.at(i);

            if (token->type == NUM_LITERAL) {

                stack.push_back(std::move(token));

            } else if (token->type == OPERATOR_PLUS || token->type == OPERATOR_MINUS) {

                if (stack.size() == 3) {
                    collapse_group(stack);
                }
                stack.push_back(std::move(token));

            } else if (token->type == OPERATOR_MULTIPLY || token->type == OPERATOR_DIVIDE) {

                // strong operators, apply precedence

                if (stack.empty()) {
                    throw std::invalid_argument("error: Stack empty, operator needs a left side argument");
                }

                if (i == tokens.size() - 1) {
                    throw std::invalid_argument("error: End of input, operator needs a right side argument");
                }

                std::unique_ptr<node> right, left;

                left = pop_back(stack);

                if (tokens[i + 1]->type == PARENTHESIS_OPEN) {

                    std::vector<std::unique_ptr<node>> subset;
                    cut_out_parenthesis(i + 1, tokens, subset);

                    auto parsed_sub_tree = parse_inner(subset);

                    right = std::move(parsed_sub_tree);

                } else {
                    right = std::move(tokens[i + 1]);
                }


                if (!is_balanced(left)) {
                    throw std::invalid_argument("error: Invalid syntax");
                }

                if (!is_balanced(right)) {
                    throw std::invalid_argument("error: Invalid syntax");
                }

                i++;

                token->left = std::move(left);
                token->right = std::move(right);
                stack.push_back(std::move(token));

            } else if (token->type == PARENTHESIS_OPEN) {

                std::vector<std::unique_ptr<node>> subset;
                cut_out_parenthesis(i, tokens, subset);

                auto parsed_sub_tree = parse_inner(subset);

                stack.push_back(std::move(parsed_sub_tree));

            } else if (token->type == PARENTHESIS_CLOSE) {

                throw std::invalid_argument("error: Unbalanced Parenthesis");
            }
        }

        if (stack.size() == 3) {
            collapse_group(stack);
        }

        if (stack.size() == 1) {
            auto back = std::move(stack.back());
            stack.pop_back();
            return back;
        } else {
            throw std::invalid_argument("error final stack size not ok. Stack size: " + std::to_string(stack.size()));
        }
    }

    export std::unique_ptr<node> parse(const std::string& input) {
        auto tokens = tokenize(input);

        return parse_inner(tokens);
    }

    my_float_t apply_operation(const std::unique_ptr<node>& root) {
        if (root->type == NUM_LITERAL) {
            return root->value;
        } else if (root->type == OPERATOR_PLUS) {
            return apply_operation(root->left) + apply_operation(root->right);
        } else if (root->type == OPERATOR_MINUS) {
            return apply_operation(root->left) - apply_operation(root->right);
        } else if (root->type == OPERATOR_MULTIPLY) {
            return apply_operation(root->left) * apply_operation(root->right);
        } else if (root->type == OPERATOR_DIVIDE) {
            return apply_operation(root->left) / apply_operation(root->right);
        } else {
            throw std::runtime_error("unknown node type");
        }
    }


    /**
     * Evaluate a math expression
     *
     * @param input math expression to be evaluated.
     * @return The result of the evaluation
     */
    export my_float_t eval(const std::string& input) {
        auto root = parse(input);
        return apply_operation(root);
    }
} // namespace parsing
