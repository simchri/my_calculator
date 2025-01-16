#include <parsing/parsing.h>
#include <view/view.h>
namespace view {

    std::string format(const parsing::mathtype& numeric_result) {
        // format double input value to minimum number of significant digits after .

        std::string result = std::to_string(numeric_result);
        result.erase(result.find_last_not_of('0') + 1, std::string::npos);

        // if result is something.0, prune the .0 as well
        if (result.back() == '.') {
            result.pop_back();
        }

        return result;
    }

    void on_new_char_entered(ViewModel& vm) {
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

    void on_enter(ViewModel& vm) {
        vm.history.emplace_back("");
    }


} // namespace view
