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
#include <cstring>
#include <cerrno>

#include <iostream>

typedef struct sockaddr_in sockaddr_in;

class Server
{
public:
    Server( int port, std::string pass);
    Server( const Server& o );
    Server &operator=( const Server& o);
    ~Server();

    void    init( void );

    int                 get_port( void ) const;
    const std::string&  get_password( void ) const;

private:
    int             port_;
    std::string     pass_;

    int             servFd_;
    sockaddr_in     addr_;

    Server( void );

public:
    class ServerException: public std::exception
    {
        std::string message_;
    public:
        ServerException( const std::string& message );
        const char* what() const throw();
        ~ServerException() throw();
    };
};

std::ostream&    operator<<(std::ostream& os, const Server& s);