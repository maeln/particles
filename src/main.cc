#include "window_handler.hh"
#include <iostream>

int main(void) {
    WindowHandler &win = WindowHandler::instance();
    win.setup();
    win.rendering_loop();

    std::cout << std::endl;
    return 0;
}
