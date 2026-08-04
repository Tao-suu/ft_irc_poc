#include "client.hpp"

Client::Client(int fd): _fd(fd) {}
Client::Client( const Client& o ): _fd(o._fd), _IP(o._IP), _nickname(o._nickname), _username(o._username) {}
Client& Client::operator=( const Client& o )
{
    if (this != &o)
    {
        _fd = o._fd;
        _nickname = o._nickname;
        _username = o._username;
        _IP = o._IP;
    }
    return *this;
}
Client::~Client( void ) {}