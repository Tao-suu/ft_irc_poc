
#ifndef MESSAGEIN_HPP
# define MESSAGEIN_HPP

# include <map>
# include <vector>
# include <string>
# include <iostream>

class MessageIn {
	
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
};

std::ostream&	operator<<(std::ostream& os, MessageIn& msg);

#endif
