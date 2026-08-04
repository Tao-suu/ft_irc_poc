#include <iostream>

#include "Server.hpp"

int main( void )
{
    Server  server(14010, "");

    try
    {
        server.init();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    std::cout << server << std::endl;
}


