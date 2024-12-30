#include <parsing/parsing.h>
#include <view/view.h>
namespace view {

    void on_new_char_entered(ViewModel& vm) {
        try {

            if (vm.input.empty()) {
                vm.current_calculation = "";
            } else {
                vm.current_calculation = vm.input + " = " + std::to_string(parsing::eval(vm.input));
            }

            // check if history is empty
            if (vm.history.empty()) {
                vm.history.push_back(vm.current_calculation);
            } else {
                vm.history.at(vm.history.size()-1) = vm.current_calculation;
            }
        } catch (...) {
        }
    }

    void on_enter(ViewModel& vm) {
        vm.history.emplace_back("");
    }


} // namespace view
