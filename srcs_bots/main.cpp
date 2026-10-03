#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <sstream> // stringstream

#include "Bot.hpp"
#include "numerics.h"

#define	PRIMSVMG			std::string("PRIVMSG ")
#define	WELCOME				std::string("001 ")
#define WEATHER_BOT_NAME	std::string("WeatherBot")
#define CHATY_BOT_NAME		std::string("ChatyBot")
#define WEATHER_BOT_SEP		(WEATHER_BOT_NAME + std::string(" "))
#define CHATY_BOT_SEP		(CHATY_BOT_NAME + std::string(" "))

void	sendHttpRequest(std::string host, std::string port, std::string &query, std::string &response)
{
    int socket_desc;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[4096];

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0)
        throw std::string("failed to create socket");

    server = gethostbyname(host.c_str());
    if (server == NULL)
        throw std::string("could Not resolve hostname :(");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(std::stoi(port));
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    if (connect(socket_desc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        throw std::string("connection failed :(");

    std::string request = "GET " + query + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
    if (send(socket_desc, request.c_str(), request.size(), 0) < 0)
        throw std::string("failed to send request...");

    int n;
    while ((n = recv(socket_desc, buffer, sizeof(buffer), 0)) > 0){
        response.append(buffer, n);
    }
    close(socket_desc);
}

void	weather_bot(Bot &bot, std::string &senderNick, std::string params)
{
	std::cout << bot << " => " << senderNick << " => " << params << std::endl;

	std::string	response;
	std::string	queryParam = (params.find(":") == 0 ? params.substr(1) : params);
	std::string	apiKey = "6703273dcb390b8667017fa539de9e05";
	std::string	query = std::string("/geo/1.0/direct?q=") + queryParam + std::string("&limit=1&appid=") + apiKey;
	sendHttpRequest("api.openweathermap.org", "80", query, response);
	std::cout << response << std::endl;
}
void	chaty_bot(Bot &bot, std::string senderNick, std::string params)
{
	std::cout << bot << " => " << senderNick << " => " << params << std::endl;
}

std::vector<std::string> split(const std::string& s, char del) {
	std::vector<std::string>	splitted;
	std::istringstream			ss(s);
	std::string					buff;

	while (getline(ss, buff, del))
		if (!buff.empty())
			splitted.push_back(buff);

	return splitted;
}

void	receiveDatas(Bot &bot)
{
    char    buffer[2048];
    std::memset(buffer, 0, 2048);

    ssize_t bytes = recv(bot.socketFd, buffer, 2048, 0);
    // if (bytes <= 0) {toRemove_.push_back(fd); return ;}
    if (bytes > 0)  bot.receiveBuffer.append(buffer, bytes); 

    size_t  pos;
    while ((pos = bot.receiveBuffer.find(SEPARATOR)) != std::string::npos)
    {
        std::string line = bot.receiveBuffer.substr(0, pos);
        bot.receiveBuffer = bot.receiveBuffer.substr(pos + SEPARATOR.size(), bot.receiveBuffer.size() - (pos + SEPARATOR.size()));

        if ((pos = line.find(WELCOME)) == 0) {
        	std::vector<std::string>	splitted = split(line, ' ');
			std::cout << bot.name << " successfuly connected to " << splitted[splitted.size() - 2] << " !" << std::endl;
        	continue ;
        }
        if ((pos = line.find(PRIMSVMG)) == std::string::npos) continue ;

        std::string cmd = line.substr(pos + PRIMSVMG.size(), line.size() - (pos + PRIMSVMG.size()));
        std::string	senderNick = line.substr(1, line.find("!") - 1);
        if ((pos = cmd.find(WEATHER_BOT_SEP)) == 0)
        	weather_bot(bot, senderNick, cmd.substr(pos + WEATHER_BOT_SEP.size() , cmd.size() - (pos + WEATHER_BOT_SEP.size())));
        else if ((pos = cmd.find(CHATY_BOT_SEP)) == 0)
        	chaty_bot(bot, senderNick, cmd.substr(pos + CHATY_BOT_SEP.size(), cmd.size() - (pos + CHATY_BOT_SEP.size())));
        else
        	std::cerr << "BAD BOT CMD" << std::endl; // Impossible (sauf si le dev est en carton...)
		// if (!cv.validateContent(line)) continue ;
		// MessageIn	msg = cv.parseContent(line);
		// exec(msg, Clients_[fd]);
        // if (std::find(toRemove_.begin(), toRemove_.end(), fd) != toRemove_.end()) break;
    }
}

void	sendDatas(Bot &bot)
{
	if (bot.sendList.size() == 0) return ;
	std::string	toSend = bot.sendList.front() + SEPARATOR;
    if (send(bot.socketFd, toSend.c_str(), toSend.size(), 0) < 0)
       throw std::string("Failed to send message from ") + bot.name + std::string("(") + bot.sendList.front() + std::string(")");
	bot.sendList.pop_front();
}

void	startBots(std::vector<Bot> &bots)
{
	std::vector<pollfd>	pollFds;

    for (std::vector<Bot>::iterator it = bots.begin(); it != bots.end(); it++) {
    	pollfd	pollFd;
        pollFd.fd = (*it).socketFd;
        pollFd.events = POLLIN | POLLOUT;
        pollFd.revents = 0;
        pollFds.push_back(pollFd);
    }

    while (1)
    {
        if (poll(&pollFds[0], pollFds.size(), -1) == -1)
        {
            if (errno == EINTR) continue;
            throw std::string("Poll error !");
        }

        for (size_t i = 0; i < pollFds.size(); i++)
        {
            if (pollFds[i].revents == 0) continue;						// Nothing append
            if (pollFds[i].revents & POLLIN) { receiveDatas(bots[i]); }	// Can receive datas
            if (pollFds[i].revents & POLLOUT) { sendDatas(bots[i]); }	// Can send datas

            // Si poll reçoit une erreur, alors on kill tous les bots (vu qu'ils sont tous liés au même server)
            if (pollFds[i].revents & (POLLHUP | POLLERR | POLLNVAL)) { throw std::string("Connection lost"); }
        }
    }
}


void	connectBots(std::vector<Bot> &bots, int ac, char **av)
{
	if (ac != 4)
	{
		std::cerr << "Missing or too many parameters" << std::endl;
		std::cerr << "Usage: " << av[0] << " IP port password" << std::endl;
		throw std::string("");
	}

	// Creating host, port & password
    std::string host(av[1]);
    std::string port(av[2]);
    std::string password(av[3]);

    // Creating server
    struct hostent *server = gethostbyname(host.c_str());
    if (server == NULL)
        throw std::string("Could not resolve hostname :(");
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(std::stoi(port));
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    // Connecting each bot
	for (std::vector<Bot>::iterator it = bots.begin(); it != bots.end(); it++)
	{
		std::cout << "Connecting " << (*it).name << " to " << inet_ntoa(serv_addr.sin_addr) << ":" << port << "..." << std::endl;

		// Creating client socket
	    (*it).socketFd = socket(AF_INET, SOCK_STREAM, 0);
	    if ((*it).socketFd < 0)
	        throw std::string("Failed to create socket for ") + (*it).name;

	    // Connecting to server
	    if (connect((*it).socketFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	        throw std::string("Connection failed :(");
   		fcntl((*it).socketFd, F_SETFL, O_NONBLOCK);
		std::cout << (*it).name << " successfuly connected to " << inet_ntoa(serv_addr.sin_addr) << ":" << port << " !" << std::endl;

		// Creating connection messages
		(*it).sendList.push_back(std::string("PASS ") + password);
		(*it).sendList.push_back(std::string("NICK ") + (*it).name);
		(*it).sendList.push_back(std::string("USER a a a a"));
	}
	std::cout << std::endl;
}

int	main(int ac, char **av)
{
	std::cout << "=========================" << std::endl;
	std::cout << "=== Welcome to bots ! ===" << std::endl;
	std::cout << "=========================" << std::endl << std::endl;

	std::vector<Bot>	bots;
	bots.push_back(Bot(WEATHER_BOT_NAME));
	bots.push_back(Bot(CHATY_BOT_NAME));

	try {
		connectBots(bots, ac, av);
		startBots(bots);
	} catch (std::string &msg) {
		std::cerr << msg << std::endl;
	}

	for (std::vector<Bot>::iterator it = bots.begin(); it != bots.end(); it++)
		if ((*it).socketFd > -1)
			close((*it).socketFd);
	return (0);
}