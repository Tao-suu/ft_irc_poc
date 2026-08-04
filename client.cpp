#include "client.hpp"

Client::Client(){}

Client::Client(const Client &src){*this = src;}

Client &Client::operator = (const Client &src)
{
    if (this != &src)
    {
        *this = src;
    }
    return (*this);
}

Client::~Client(){}

int             Client::GetFd(){return _fd;}
std::string     Client::GetIP(){return _IP;}
std::string     Client::GetNickname(){return _nickname;}
std::string     Client::GetUsername(){return _username;}

void            Client::SetFd(int fd){_fd = fd;}
void            Client::SetIpAdd(std::string IP){_IP = IP;}
void            Client::SetNickname(std::string nickname){_nickname = nickname;}
void            Client::SetUsername(std::string username){_username = username;}