#pragma once

#include <string>

class Client
{
	private:
		int  _fd;
		std::string _IP;
        std::string _nickname;
        std::string _username;
        std::string _realname;

		bool		nick_set;
		
    public:
        std::string _in_buffer;
        std::string _out_buffer;

		bool		pass_ok;
		bool		pass_done;
        bool        user_ok;
        bool        nick_ok;

		bool		registered;

        Client( int fd = 0 );
        Client(const Client &o);
        ~Client();
        Client &operator=(const Client &o);
        
		int          GetFd() const;
        std::string  GetIP() const;
        std::string  GetNickname() const;
        std::string  GetUsername() const;
        std::string  GetRealname() const;

		// void		set_pass_ok(bool val);

		void    SetFd(int fd);
        void    SetIpAdd(std::string IP);
        void    SetNickname(std::string nickname);
        void    SetUsername(std::string username);
        void    SetRealname(std::string realname);
};
