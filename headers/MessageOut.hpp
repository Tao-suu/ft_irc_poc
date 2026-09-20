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

# pragma once

# include <vector>
# include <string>
# include <iostream>

# include "client.hpp"

class Client;

class MessageOut {

private:
	// Attributes
	std::string				message_;
	std::vector<int>		targets_;

	// Methods

public:
	// Attributes

	// Constructors / Destructor
	MessageOut( void );
	MessageOut( std::string message, std::vector<int> clients = std::vector<int>() );
	MessageOut( MessageOut const &other );
	~MessageOut( void );

	// Operators overload
	MessageOut	&operator=( MessageOut const &other );

	// Getters
	const std::vector<int>		getTargets( void ) const;
	const std::string			getMessage( void ) const;

	// Setters
	void						setMessage( std::string message );
	void						addTarget( int fd );

	// Methods
	void						send( void );
};