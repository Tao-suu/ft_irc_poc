#pragma once

#include <string>


class Client
{
    private:
	    int  _fd;
	    std::string _IP;
        std::string _nickname;
        std::string _username;

        
    public:
        std::string _in_buffer;

	    Client( int fd = 0 );
        Client(const Client &src);
        Client &operator = (const Client &src);
        ~Client();

	    int     GetFd(){return _fd;}
        std::string  GetIP(){return _IP;}
        std::string  GetNickname(){return _nickname;}
        std::string  GetUsername(){return _username;}

	    void    SetFd(int fd){_fd = fd;}
	    void    SetIpAdd(std::string IP){_IP = IP;}
        void    SetNickname(std::string nickname){_nickname = nickname;}
        void    SetUsername(std::string username){_username = username;}
};