#include <iostream>

#include "Server.hpp"

int main( void )
{
    Server  server(14010, "");

    try
    {
        server.init();
        std::cout << server << std::endl;
        server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    std::cout << server.get_ip() << std::endl;
}
