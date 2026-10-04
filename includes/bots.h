#pragma once

#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <sstream> // stringstream
#include <cstring>
#include <cstdlib>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "Bot.hpp"
#include "numerics.h"

#define	PRIMSVMG			std::string("PRIVMSG ")
#define	WELCOME				std::string("001 ")
#define WEATHER_BOT_NAME	std::string("WeatherBot")
#define CHATY_BOT_NAME		std::string("ChatyBot")
#define WEATHER_BOT_SEP		(WEATHER_BOT_NAME + std::string(" "))
#define CHATY_BOT_SEP		(CHATY_BOT_NAME + std::string(" "))
#define DATE_FORMAT			std::string("%d/%m/%Y")
#define HOUR_FORMAT			std::string("%H:%M:%S")

// serverTools.cpp
void						createServerAddr(std::string &host, std::string &port, struct sockaddr_in &serv_addr);
void						connectToServer(std::string &host, std::string &port, int &socketFd);
void						sendHttpRequest(std::string host, std::string port, std::string &query, std::string &response);
void	                    sendHttpRequest(std::string host, std::string port, std::string &query, std::string &response, std::string body, std::string api_key);
void						sendPrivmsg(Bot &bot, std::string &senderNick, std::string msg);

//tools.cpp
std::vector<std::string>	split(const std::string& s, char del);
void						extractFromString(std::string &dest, std::string &src, std::string start, std::string end);
std::string					formatDateTime(time_t timeValue, std::string format);
