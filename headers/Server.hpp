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
#include <stack>

#include "client.hpp"
#include "CommandValidator.hpp"
#include "Error.hpp"
#include "numerics.h"
#include "channel.hpp"
#include "MessageOut.hpp"

typedef struct sockaddr_in sockaddr_in;
typedef struct pollfd pollfd;

class Error;
class Channel;

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
    void                handleClientWrite( int fd );

private:
    int             port_;
    std::string     pass_;

    int             servFd_;
    sockaddr_in     addr_;

    std::vector<pollfd>     pollfds_;
    std::vector<int>        toRemove_;
    std::map<int, Client>   Clients_;

    std::vector<Channel>    Channels_;

	CommandValidator	cv;

    std::deque<MessageOut>  message_stack;

    Server( void );
	void				exec(MessageIn &msg, Client& sender);

	void				pass(MessageIn &msg, Client& cl);
	void				nick(MessageIn &msg, Client& cl);
	void				user(MessageIn &msg, Client& cl);
	
	void				join(MessageIn& msg, Client& cl);
	// void				kick();
	// void				invite();
	// void				mode();
	// void				privmsg();
	void				ping(MessageIn& msg, Client& cl);

	bool 				            is_nickname_exist(std::string nick);
	void				            send_error(Error &e);
	void				            sendWelcome(Client &cl);
    void                            send_all( void );

    bool                            is_channel_exist(std::string name);
    std::vector<Channel>::iterator  get_channel(std::string name);

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
