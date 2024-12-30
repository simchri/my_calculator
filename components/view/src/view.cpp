#include <parsing/parsing.h>
#include <view/view.h>
namespace view {

    void on_new_char_entered(ViewModel& vm) {
        try {
            vm.current_calculation = vm.input + " = " + std::to_string(parsing::eval(vm.input));
        } catch (...) {
        }
    }

    void on_enter(ViewModel& vm) {
        vm.history.push_back(vm.current_calculation);
    }


} // namespace view
