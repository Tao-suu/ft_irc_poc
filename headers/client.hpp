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
        Client(const Client &o);
        ~Client();
        Client &operator=(const Client &o);

	int          GetFd();
        std::string  GetIP();
        std::string  GetNickname();
        std::string  GetUsername();

	void    SetFd(int fd);
        void    SetIpAdd(std::string IP);
        void    SetNickname(std::string nickname);
        void    SetUsername(std::string username);
};