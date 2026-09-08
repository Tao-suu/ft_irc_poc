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
	std::vector<Client*>	targets_;

	// Methods

public:
	// Attributes

	// Constructors / Destructor
	MessageOut( void );
	MessageOut( std::string message, std::vector<Client*> clients = std::vector<Client*>() );
	MessageOut( MessageOut const &other );
	~MessageOut( void );

	// Operators overload
	MessageOut	&operator=( MessageOut const &other );

	// Getters
	const std::vector<Client*>	getTargets( void ) const;
	const std::string			getMessage( void ) const;

	// Setters
	void						setMessage( std::string message );
	void						addTarget( Client* cl );

	// Methods
	void						send( void );
};