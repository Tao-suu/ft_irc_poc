/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageIn.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: picheval <picheval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 14:52:18 by picheval          #+#    #+#             */
/*   Updated: 2026/08/31 15:33:13 by picheval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEIN_HPP
# define MESSAGEIN_HPP

# include <map>
# include <vector>
# include <string>
# include <iostream>

class MessageIn {

private:
	// Attributes

	// Methods

public:
	// Attributes
	std::map<std::string, std::string>		tags;
	std::string								cmdName;
	std::vector< std::vector<std::string> >	args;

	// Constructors / Destructor
	MessageIn(std::map<std::string, std::string> tags = std::map<std::string, std::string>(),
		std::string cmdName = "",
		std::vector< std::vector<std::string> > args = std::vector< std::vector<std::string> >());
	MessageIn(MessageIn const &other);
	~MessageIn(void);

	// Operators overload
	MessageIn	&operator=(MessageIn const &other);

	// Getters

	// Setters

	// Methods

};

std::ostream&	operator<<(std::ostream& os, MessageIn& msg);

#endif
