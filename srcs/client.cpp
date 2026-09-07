#include "client.hpp"

Client::Client(int fd): _fd(fd) {
	pass_ok = false;
	registered = false;
	cap_process = 0;
}

Client::Client( const Client& o ): _fd(o._fd), _IP(o._IP), _nickname(o._nickname), _username(o._username) {}

Client::~Client( void ) {}

Client& Client::operator=( const Client& o )
{
    if (this != &o)
    {
        _fd = o._fd;
        _nickname = o._nickname;
        _username = o._username;
        _IP = o._IP;
        pass_ok = o.pass_ok;
        registered = o.registered;
        cap_process = o.cap_process;
    }
    return *this;
}

int             Client::GetFd(){return _fd;}
std::string     Client::GetIP(){return _IP;}
std::string     Client::GetNickname(){return _nickname;}
std::string     Client::GetUsername(){return _username;}

void            Client::SetFd(int fd){_fd = fd;}
void            Client::SetIpAdd(std::string IP){_IP = IP;}
void            Client::SetNickname(std::string nickname){_nickname = nickname;}
void            Client::SetUsername(std::string username){_username = username;}
void            Client::SetRealname(std::string realname){_realname = realname;}
