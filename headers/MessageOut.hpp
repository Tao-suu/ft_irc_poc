
# pragma once

# include <vector>
# include <string>
# include <iostream>

# include "Client.hpp"

class Client;

class MessageOut {

private:
	// Attributes
	std::string				message_;
	std::vector<int>		targets_;

public:
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
};