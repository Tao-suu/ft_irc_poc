/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numerics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbouchar <lbouchar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 15:46:56 by lbouchar          #+#    #+#             */
/*   Updated: 2026/09/08 11:45:37 by lbouchar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_H
# define NUMERICS_H

# include <string>

# define SEPARATOR										std::string("\r\n")

/****RPL****/
# define RPL_WELCOME(client)							std::string("001 ") + client + " :Welcome to the ft_irc Network"
# define RPL_YOURHOST(client, servername, version)		std::string("002 ") + client + " :Your host is " + servername + ", running version " + version
# define RPL_CREATED(client, datetime)					std::string("003 ") + client + " :This server was created " + datetime

// TODO: add [<channel modes with a parameter>]
# define RPL_MYINFO(client, servername, version, modes)	std::string("004 ") + client + " " + servername + " " + version + " " + modes

# define RPL_TOPIC(client, channel, topic)				std::string("332 ") + client + " " + channel + ":" + topic
# define RPL_TOPICWHOTIME(client, channel, nick, setat)	std::string("333 ") + client + " " + channel + " " + nick + " " + setat

/****ERR****/

# define ERR_UNKNOWNERROR(client, command)				std::string("400 ") + client + " " + command + " :Could not process invalid parameters"
# define ERR_NONICKNAMEGIVEN(client)					std::string("431 ") + client + " :No nickname given"
# define ERR_ERRONEUSNICKNAME(client, nick)				std::string("432 ") + client + " " + nick + " :Erroneus nickname"
# define ERR_NICKNAMEINUSE(client, nick)				std::string("433 ") + client + " " + nick + " :Nickname is already in use"
# define ERR_USERNOTINCHANNEL(client, nick, channel)    std::string("441 ") + client + " " + nick + " " + channel + " :They aren't on that channel"
# define ERR_NOTONCHANNEL(client, channel)              std::string("442 ") + client + " " + channel + " :You're not on that channel"
# define ERR_NOTREGISTERED(client)						std::string("451 ") + client + " :You have not registered"
# define ERR_NEEDMOREPARAMS(client, command)			std::string("461 ") + client + " " + command + " :Not enough parameters"
# define ERR_ALREADYREGISTERED(client)					std::string("462 ") + client + " :You may not reregister"
# define ERR_PASSWDMISMATCH(client)						std::string("464 ") + client + " :Password incorrect"
# define ERR_CHANNELISFULL(client, channel)				std::string("471 ") + client + " " + channel + " :Cannot join channel - Channel is full"
# define ERR_INVITEONLYCHAN(client, channel)			std::string("473 ") + client + " " + channel + " :Cannot join channel - Channel is set to invite only"
# define ERR_BANNEDFROMCHAN(client, channel)			std::string("474 ") + client + " " + channel + " :Cannot join channel - Banned from channel"
# define ERR_BADCHANNELKEY(client, channel)				std::string("475 ") + client + " " + channel + " :Cannot join channel - Bad channel key"
# define ERR_CHANOPRIVSNEEDED(client, channel)          std::string("482 ") + client + " " + channel + " :You're not channel operator"

#endif