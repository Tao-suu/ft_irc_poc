class Client
{
    private:
	    int  _fd;
	    std::string _IP;
        std::string _nickname;
        std::stirng _username;

    public:
	    Client(){};
        Client(const Client &src);
        Client &operator = (const Client &src);
        ~Client(){};

	    int     GetFd(){return _fd;}
        string  GetIP(){return _IP;}
        string  GetNickname(){return nickname;}
        string  GetUsername(){return username;}

	    void    SetFd(int fd){_fd = fd;}
	    void    SetIpAdd(std::string IP){_IP = IP;}
        void    SetNickname(std::string nickname){_nickname = nickname;}
        void    SetUsername(std::string username){_username = username;}
};