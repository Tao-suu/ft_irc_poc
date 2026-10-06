#include "Bot.hpp"

/******************/
/* Public methods */
/******************/
// Constructors / Destructor
Bot::Bot(std::string name): name(name), socketFd(-1) {}
Bot::Bot(Bot const &other) { *this = other; }
Bot::~Bot(void) { }

// Operators overload
Bot	&Bot::operator=(Bot const &other)
{
	if (&other != this)	{ name = other.name; }
	return (*this);
}
std::ostream&	operator<<(std::ostream& os, Bot& bot)
{
	os << bot.name << " (" << bot.socketFd << ")";
	return (os);
}

