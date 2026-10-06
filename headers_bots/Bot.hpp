#ifndef BOT_HPP
# define BOT_HPP

# include <vector>
# include <deque>
# include <iostream>

typedef struct pollfd pollfd;

class Bot {

public:
	// Attributes
	std::string					name;
	int							socketFd;
	std::deque<std::string>		sendList;
	std::string					receiveBuffer;

	// Constructors / Destructor
	Bot(std::string name = "");
	Bot(Bot const &other);
	~Bot(void);

	// Operators overload
	Bot	&operator=(Bot const &other);
};

std::ostream&	operator<<(std::ostream& os, Bot& msg);

#endif
