#pragma once

#include <string>
#include <vector>

namespace view {
    struct ViewModel {
        std::vector<std::string> history;
        std::string input;
        std::string current_calculation;
    };

    void on_new_char_entered(ViewModel& vm);
    void on_enter(ViewModel& vm);

} // namespace view
