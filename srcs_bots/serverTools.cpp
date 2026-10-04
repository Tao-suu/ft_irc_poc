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

void	sendHttpRequest(std::string host, std::string port, std::string &query, std::string &response, std::string body, std::string api_key)
{
	int socketFd;
	char buffer[4096];

	connectToServer(host, port, socketFd);

    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    SSL_CTX_set_verify(
        ctx,
        SSL_VERIFY_PEER,
        NULL
    );
    SSL_CTX_set_default_verify_paths(ctx);
    SSL* ssl = SSL_new(ctx);
    SSL_set_tlsext_host_name(ssl, host.c_str());
    SSL_set_fd(ssl, socketFd);
    if (SSL_connect(ssl) != 1)
    {
        ERR_print_errors_fp(stderr);
        return;
    }

	std::stringstream ss;
	ss << body.size();
	std::string str = ss.str();
	std::string request = "POST " + query + " HTTP/1.1\r\nHost: " + host
	+ "\r\nAuthorization: Bearer " + api_key + "\r\nContent-Type: application/json\r\nContent-Length: " + str + "\r\nConnection: close\r\n\r\n" + body;
	
	SSL_write(
        ssl,
        request.data(),
        request.size()
    );

	int n;
	while ((n = SSL_read(ssl, buffer, sizeof(buffer))) > 0)
		response.append(buffer, n);
	
	SSL_shutdown(ssl);
    SSL_free(ssl);

	close(socketFd);

    SSL_CTX_free(ctx);
}

void	sendPrivmsg(Bot &bot, std::string &senderNick, std::string msg)
{
	bot.sendList.push_back(std::string("PRIVMSG ") + senderNick + std::string(" :") + msg);
}