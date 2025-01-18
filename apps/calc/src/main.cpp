#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
import view;


int main() {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();

    auto vm = view::ViewModel{.history = {}, .input = "", .current_calculation = ""};

    auto input_box = Input(&vm.input, "input", InputOption::Default());

    input_box |= CatchEvent([&](const Event& event) {
        if (event == Event::Character('q')) {
            screen.ExitLoopClosure()();
            return true;
        }

        view::on_new_char_entered(vm);

        bool handled = false;

        if (event == Event::Character('\n')) {

            view::on_enter(vm);

            handled = true; // hitting "enter" should do nothing else! (Not also add a
                            // newline in the input field)
        }

        if (event != Event::Custom) {
            // trigger a screen update
            // (but only if the event we are catching is not a screen-update event!
            // otherwise infinite loop)
            screen.PostEvent(Event::Custom);
        }

        return handled;
    });

    auto component = Container::Vertical({
            input_box,
    });

    auto renderer = Renderer(component, [&] {
        return vbox({
                window(text("results"),
                       vbox(
                               // Render each line's input field.
                               [&]() -> std::vector<Element> {
                                   std::vector<Element> elements;
                                   elements.reserve(vm.history.size());
                                   for (auto& history_item : vm.history) {
                                       elements.push_back(paragraph(history_item));
                                   }
                                   return elements;
                               }())) |
                        flex,
                input_box->Render() | border,
        });
    });

    screen.Loop(renderer);
}
