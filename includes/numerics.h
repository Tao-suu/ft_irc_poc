/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numerics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laura <laura@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 15:46:56 by lbouchar          #+#    #+#             */
/*   Updated: 2026/10/02 14:14:56 by laura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_H
# define NUMERICS_H

# include <string>

# define SEPARATOR										std::string("\r\n")

/*****SOURCE PREFIX*****/
# define PREFIX(nick, user, host)                       std::string(":") + nick + "!" + user + "@" + host

/****RPL****/
# define RPL_WELCOME(client)							std::string("001 ") + client + " :Welcome to the ft_irc Network"
# define RPL_YOURHOST(client, servername, version)		std::string("002 ") + client + " :Your host is " + servername + ", running version " + version
# define RPL_CREATED(client, datetime)					std::string("003 ") + client + " :This server was created " + datetime

// TODO: add [<channel modes with a parameter>]
# define RPL_MYINFO(client, servername, version, modes)	std::string("004 ") + client + " " + servername + " " + version + " " + modes

# define RPL_LISTSTART(client)                          std::string("321 ") + client + " Channel :Users  Name" 
# define RPL_LIST(client, channel, ccount, topic)       std::string("322 ") + client + " " + channel + " " + ccount + " :" + topic 
# define RPL_LISTEND(client)                            std::string("323 ") + client + " :End of /LIST"

# define RPL_NOTOPIC(client, channel)                   std::string("331 ") + client + " " + channel + " :No topic is set"
# define RPL_TOPIC(client, channel, topic)				std::string("332 ") + client + " " + channel + " :" + topic
# define RPL_TOPICWHOTIME(client, channel, nick, setat)	std::string("333 ") + client + " " + channel + " " + nick + " " + setat
# define RPL_INVITING(client, nick, channel)            std::string("341 ") + client + " " + nick + " " + channel
# define RPL_ENDOFNAMES(client, channel)                std::string("366 ") + client + " " + channel + " :End of /NAMES list"

/*****MSG default*******/
// doc say send messsage but not give it so i create those
# define DFL_JOIN(client, channel)                      std::string(client) + " is joining the channel " + channel
# define DFL_KICK(client, target, channel, comment)     std::string(client) + " has kicked " + target + " to the channel " + channel + " " + comment

# define DFL_INVITE(client, target, channel)            std::string(client) + " has invited " + target + " to the channel " + channel
# define DFL_SETINVITEMODE(client, channel)             std::string(client) + " has set the Invite-Only to the channel " + channel
# define DFL_REMOVEINVITEMODE(client, channel)          std::string(client) + " has remove the Invite-Only to the channel " + channel

# define DFL_TAKEOPERATORPRIVILEGE(client, target, channel) std::string(client) + " has set operator privilege " + target + " to the channel " + channel
# define DFL_GIVEOPERATORPRIVILEGE(client, target, channel) std::string(client) + " has remove operator privilege " + target + " to the channel " + channel

# define DFL_SETUSERLIMIT(client, limit, channel)       std::string(client) + " has set the user limit at " + limit + " to the channel " + channel
# define DFL_REMOVEUSERLIMITMODE(client, channel)       std::string(client) + " has remove the USER-LIMIT mMode to the channel " + channel

# define DFL_SETKEY(client, channel)                    std::string(client) + " has set a key to the channel " + channel
# define DFL_REMOVEKEYMODE(client, channel)             std::string(client) + " has remove the KEY Mode to the channel " + channel

# define DFL_SETTOPIC(nick, user, host, topic, channel) PREFIX(nick, user, host) + " TOPIC " + channel + " :" + topic
# define DFL_REMOVETOPICMODE(client, channel)           std::string(client) + " has remove the KEY Mode to the channel " + channel
/****ERR****/

# define ERR_UNKNOWNERROR(client, command)				std::string("400 ") + client + " " + command + " :Could not process invalid parameters"
# define ERR_NOSUCHNICK(client, target)          		std::string("401 ") + client + " " + target + " :No such nick/channel"
# define ERR_NOSUCHCHANNEL(client, channel)             std::string("403 ") + client + " " + channel + " :No such channel"
# define ERR_CANNOTSENDTOCHAN(client, channel)          std::string("404 ") + client + " " + channel + " :Cannot send to channel"
# define ERR_NORECIPIENT(client, command)         		std::string("411 ") + client + " :No recipient (" + command + ")"
# define ERR_NOTEXTTOSEND(client)                		std::string("412 ") + client + " :No text to send"
# define ERR_UNKNOWCOMMAND(client, command)             std::string("421 ") + client + " " + command + " :Unknow command"
# define ERR_NONICKNAMEGIVEN(client)					std::string("431 ") + client + " :No nickname given"
# define ERR_ERRONEUSNICKNAME(client, nick)				std::string("432 ") + client + " " + nick + " :Erroneus nickname"
# define ERR_NICKNAMEINUSE(client, nick)				std::string("433 ") + client + " " + nick + " :Nickname is already in use"
# define ERR_USERNOTINCHANNEL(client, nick, channel)    std::string("441 ") + client + " " + nick + " " + channel + " :They aren't on that channel"
# define ERR_NOTONCHANNEL(client, channel)              std::string("442 ") + client + " " + channel + " :You're not on that channel"
# define ERR_USERONCHANNEL(client, nick, channel)       std::string("443 ") + client + " " + nick + " " + channel + " :is already on channel"
# define ERR_NOTREGISTERED(client)						std::string("451 ") + client + " :You have not registered"
# define ERR_NEEDMOREPARAMS(client, command)			std::string("461 ") + client + " " + command + " :Not enough parameters"
# define ERR_ALREADYREGISTERED(client)					std::string("462 ") + client + " :You may not reregister"
# define ERR_PASSWDMISMATCH(client)						std::string("464 ") + client + " :Password incorrect"
# define ERR_CHANNELISFULL(client, channel)				std::string("471 ") + client + " " + channel + " :Cannot join channel - Channel is full"
# define ERR_INVITEONLYCHAN(client, channel)			std::string("473 ") + client + " " + channel + " :Cannot join channel - Channel is set to invite only"
# define ERR_BADCHANNELKEY(client, channel)				std::string("475 ") + client + " " + channel + " :Cannot join channel - Bad channel key"
# define ERR_BADCHANNELMASK(client, channel)			std::string("476 ") /*+ client*/ + " " + channel + " :Bad channel mask"
# define ERR_CHANOPRIVSNEEDED(client, channel)          std::string("482 ") + client + " " + channel + " :You're not channel operator"


/*****COMMAND ECHO*****/
# define MSG_NICK(prefix, newnick)                      prefix + " NICK :" + newnick

#endif