#include <string>

class Client
{
    private:
	    int  _fd;
	    std::string _IP;
        std::string _nickname;
        std::string _username;

    public:
	    Client();
        Client(const Client &src);
        Client &operator = (const Client &src);
        ~Client();

	    int          GetFd();
        std::string  GetIP();
        std::string  GetNickname();
        std::string  GetUsername();

	    void    SetFd(int fd);
	    void    SetIpAdd(std::string IP);
        void    SetNickname(std::string nickname);
        void    SetUsername(std::string username);
};