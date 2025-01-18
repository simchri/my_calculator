module;

#include <string>
#include <vector>

import parsing;

export module view;

namespace view {
    export struct ViewModel {
        std::vector<std::string> history;
        std::string input;
        std::string current_calculation;
    };


    std::string format(const parsing::my_float_t& numeric_result) {

        // format double input value to minimum number of significant digits after .
        std::string result = std::to_string(numeric_result);
        result.erase(result.find_last_not_of('0') + 1, std::string::npos);

        // if result is something.0, prune the .0 as well
        if (result.back() == '.') {
            result.pop_back();
        }

        return result;
    }

    export void on_new_char_entered(ViewModel& vm) {
        try {

            if (vm.input.empty()) {
                vm.current_calculation = "";
            } else {
                vm.current_calculation = vm.input + " = " + format(parsing::eval(vm.input));
            }

            // check if history is empty
            if (vm.history.empty()) {
                vm.history.push_back(vm.current_calculation);
            } else {
                vm.history.at(vm.history.size() - 1) = vm.current_calculation;
            }
        } catch (...) {
        }
    }

    export void on_enter(ViewModel& vm) {
        vm.history.emplace_back("");
    }


} // namespace view
