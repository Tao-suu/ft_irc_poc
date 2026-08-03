#pragma once

#include <vector>
#include <string>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct sockaddr_in sockaddr_in;

class Server
{
public:
    Server( int port, std::string pass);
    Server( const Server& o );
    Server &operator=( const Server& o);
    ~Server();

    void    init( void );

private:
    int             port_;
    std::string     pass_;

    int             servFd_;
    sockaddr_in     addr_;

    Server( void );
};