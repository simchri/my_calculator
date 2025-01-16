#include <parsing/parsing.h>

namespace parsing {


    bool node::operator==(const node& other) const {
        if ((left == nullptr) != (other.left == nullptr) || (right == nullptr) != (other.right == nullptr)) {
            return false;
        }

        if (type != other.type || value != other.value) {
            return false;
        }

        if (left && *left != *other.left) {
            return false;
        }

        if (right && *right != *other.right) {
            return false;
        }

        return true;
    }

    std::vector<std::unique_ptr<node>> tokenize(const std::string& input) {

        enum LexerState { LITERAL, OPERATOR };

        std::string literal;

        LexerState state;

        std::vector<std::unique_ptr<node>> tokens;

        size_t str_ind = 0;
        int str_ind_section_start = 0;
        auto str_ind_end = input.length() - 1;
        while (str_ind <= str_ind_end) {

            if (!std::isdigit(input[str_ind])) {
                state = OPERATOR;
            } else {
                state = LITERAL;
            }

            if (state == LITERAL) {

                if (str_ind >= str_ind_end || !std::isdigit(input.at(str_ind + 1))) {

                    literal = input.substr(str_ind_section_start, str_ind - str_ind_section_start + 1);
                    auto new_token = std::make_unique<node>(node{.type = NUM_LITERAL, .value = std::stoi(literal)});
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

    std::unique_ptr<node> parse(const std::string& input) {
        auto tokens = tokenize(input);

        if (tokens.size() == 1) {
            if (tokens[0]->type == NUM_LITERAL) {
                return std::move(tokens[0]);
            } else {
                throw std::invalid_argument("single token provided can not be an operator.");
            }
        }

        std::vector<std::unique_ptr<node>> stack;
        stack.reserve(tokens.size());

        for (std::size_t i = 0; i != tokens.size(); ++i) {
            auto& token = tokens[i];

            if (token->type == NUM_LITERAL) {
                stack.push_back(std::move(token));
            } else if (token->type == OPERATOR_PLUS || token->type == OPERATOR_MINUS) {

                // weak operators
                if (stack.size() == 3) {
                    collapse_group(stack);
                }
                stack.push_back(std::move(token));

            } else if (token->type == OPERATOR_MULTIPLY || token->type == OPERATOR_DIVIDE) {

                // strong operators, apply precedence

                // check that at least one item is on the stack:
                if (stack.empty()) {
                    throw std::invalid_argument("Missing operand!");
                }

                // check that we are not at the end of the input:
                if (i == tokens.size() - 1) {
                    throw std::invalid_argument("Missing operand!");
                }

                // get left element (stack) and right element (input)
                auto left = pop_back(stack);
                auto right = std::move(tokens[i + 1]);
                i++;

                token->left = std::move(left);
                token->right = std::move(right);
                stack.push_back(std::move(token));
            }
        }

        if (stack.size() == 3) {
            collapse_group(stack);
        }

        if (stack.size() == 1) {
            return std::move(stack.back());
        } else {
            throw std::invalid_argument("final stack size not ok:" + std::to_string(stack.size()));
        }
    }

    int apply_operation(const std::unique_ptr<node>& root) {
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
    int eval(const std::string& input) {
        auto root = parse(input);
        return apply_operation(root);
    }


} // namespace parsing
