#pragma once

#include <string>

class Client
{
	private:
		int  _fd;
		std::string _IP;
        std::string _nickname;
        std::string _username;

		bool		nick_set;
		
	public:
        std::string _in_buffer;

		bool		pass_ok;
		bool		registered;

        Client( int fd = 0 );
        Client(const Client &o);
        ~Client();
        Client &operator=(const Client &o);
        
		int          GetFd();
        std::string  GetIP();
        std::string  GetNickname();
        std::string  GetUsername();

		// void		set_pass_ok(bool val);

		void    SetFd(int fd);
        void    SetIpAdd(std::string IP);
        void    SetNickname(std::string nickname);
        void    SetUsername(std::string username);
};
