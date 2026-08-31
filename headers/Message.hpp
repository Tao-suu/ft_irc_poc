/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: picheval <picheval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 14:52:18 by picheval          #+#    #+#             */
/*   Updated: 2026/08/31 15:33:13 by picheval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <map>
# include <vector>
# include <string>
# include <iostream>

class Message {

private:
	// Attributes

	// Methods

public:
	// Attributes
	std::map<std::string, std::string>	tags;
	std::string							cmdName;
	std::vector<std::string>			args;

	// Constructors / Destructor
	Message(std::map<std::string, std::string> tags = std::map<std::string, std::string>(),
		std::string cmdName = "",
		std::vector<std::string> args = std::vector<std::string>());
	Message(Message const &other);
	~Message(void);

	// Operators overload
	Message	&operator=(Message const &other);

	// Getters

	// Setters

	// Methods

};

std::ostream&	operator<<(std::ostream& os, Message& msg);

#endif
