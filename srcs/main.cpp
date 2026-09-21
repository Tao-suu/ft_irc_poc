#include <iostream>

#include "Server.hpp"


bool args_valid(int argc, char **argv, int &port) {
    if (argc < 3) {
        std::cerr << "Too few args\n";
        return 0;
    }

    if (argc > 3) {
        std::cerr << "Too many args\n";
        return 0;
    }

    std::string port_val = argv[1];
    for (size_t i = 0; i < port_val.size(); i++) 
        if (!::isdigit(port_val[i])) {
            std::cerr << "invalid port\n";
            return false;
        }

    port = ::atoi(port_val.c_str());
    if (port < 1 || port > 65535) {
        std::cerr << "port out of range\n";
        return false;
    } 

    return true;
}


int main( int argc, char **argv )
{
    int     port;
    if (!args_valid(argc, argv, port)) return 1;
    Server  server(port, argv[2]);

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
