#include <iostream>
#include "window_handler.h"

int main(void)
{	
    WindowHandler& win = WindowHandler::instance();
    win.setup();
    win.rendering_loop();
    
    std::cout << std::endl;
    return 0;
}
