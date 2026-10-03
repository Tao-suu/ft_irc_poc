#include "bots.h"

void	createServerAddr(std::string &host, std::string &port, struct sockaddr_in &serv_addr)
{
	struct hostent *server;

	server = gethostbyname(host.c_str());
	if (server == NULL)
		throw std::string("Could not resolve hostname ") + host + std::string(" :(");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(std::atoi(port.c_str()));
	std::memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
}
void	connectToServer(std::string &host, std::string &port, int &socketFd)
{
	struct sockaddr_in serv_addr;

	std::memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	createServerAddr(host, port, serv_addr);
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0)
		throw std::string("Failed to create socket");

	if (connect(socketFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		throw std::string("Connection failed :(");
}

void	sendHttpRequest(std::string host, std::string port, std::string &query, std::string &response)
{
	int socketFd;
	char buffer[4096];

	connectToServer(host, port, socketFd);

	std::string request = "GET " + query + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
	if (send(socketFd, request.c_str(), request.size(), 0) < 0)
		throw std::string("Failed to send request...");

	int n;
	while ((n = recv(socketFd, buffer, sizeof(buffer), 0)) > 0)
		response.append(buffer, n);
	close(socketFd);
}

void	sendPrivmsg(Bot &bot, std::string &senderNick, std::string msg)
{
	bot.sendList.push_back(std::string("PRIVMSG ") + senderNick + std::string(" :") + msg);
}