/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: picheval <picheval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 14:51:33 by picheval          #+#    #+#             */
/*   Updated: 2026/08/31 16:02:09 by picheval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

/******************/
/* Public methods */
/******************/
// Constructors / Destructor
Message::Message(std::map<std::string, std::string> tags,
		std::string cmdName,
		std::vector<std::string> args): tags(tags), cmdName(cmdName), args(args)
{
}
Message::Message(Message const &other)
{
	*this = other;
}
Message::~Message(void)
{
}

// Operators overload
Message	&Message::operator=(Message const &other)
{
	if (&other != this)
	{
		tags = other.tags;
		cmdName = other.cmdName;
		args = other.args;
	}
	return (*this);
}
std::ostream&	operator<<(std::ostream& os, Message& msg)
{
	os << "tags:";
	for (std::map<std::string, std::string>::iterator it = msg.tags.begin(); it != msg.tags.end(); it++)
		os << " \"" << (*it).first << "\"=\"" << (*it).second << "\"";
	os << std::endl;
	os << "cmd: " << msg.cmdName << std::endl;
	os << "params:";
	for (std::vector<std::string>::iterator it = msg.args.begin(); it != msg.args.end(); it++)
		os << " \"" << *it << "\"";
	os << std::endl;
	return (os);
}


// Getters

// Setters

// Methods

/*******************/
/* Private methods */
/*******************/
