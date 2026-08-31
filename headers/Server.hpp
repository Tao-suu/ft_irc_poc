#pragma once

#include <vector>
#include <string>
#include <map>

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

#include "client.hpp"
#include "CommandValidator.hpp"

typedef struct sockaddr_in sockaddr_in;
typedef struct pollfd pollfd;

class Server
{
public:
    Server( int port, std::string pass);
    Server( const Server& o );
    Server &operator=( const Server& o);
    ~Server();

    void    init( void );
    void    run ( void );

    int                 get_port( void ) const;
    const std::string&  get_password( void ) const;
    const std::string   get_ip( void ) const;

    void                acceptNewClient( void );
    void                handleClientData( int fd );

private:
    int             port_;
    std::string     pass_;

    int             servFd_;
    sockaddr_in     addr_;

    std::vector<pollfd>     pollfds_;
    std::vector<int>        toRemove_;
    std::map<int, Client>   Clients_;

	CommandValidator	cv;

    Server( void );
	void				exec(Message &msg);

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