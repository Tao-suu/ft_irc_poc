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
		std::vector< std::vector<std::string> > args): tags(tags), cmdName(cmdName), args(args)
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
	os << "cmd: " << msg.cmdName << std::endl;
	os << "tags:" << std::endl;
	for (std::map<std::string, std::string>::iterator it = msg.tags.begin(); it != msg.tags.end(); it++)
		os << "\t\"" << (*it).first << "\"=\"" << (*it).second << "\"" << std::endl;
	os << "params:" << std::endl;
	for (std::vector< std::vector<std::string> >::iterator it1 = msg.args.begin(); it1 != msg.args.end(); it1++)
	{
		os << "\t";
		for (std::vector<std::string>::iterator it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
		{
			if (it2 != (*it1).begin())
				os << " ";
			os << "\"" << *it2 << "\"";
		}
		os << std::endl;
	}
	return (os);
}


// Getters

// Setters

// Methods

/*******************/
/* Private methods */
/*******************/
