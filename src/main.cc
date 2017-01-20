#include <iostream>
#include "window_handler.hh"

int main(void)
{	
    WindowHandler& win = WindowHandler::instance();
    win.setup();
    win.rendering_loop();
    
    std::cout << std::endl;
    return 0;
}
