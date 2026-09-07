#include "Error.hpp"


Error::Error(Client client, std::string msg): _client(client), _msg(msg) {}

Error::Error(const Error &copy)
{
    this->_client = copy._client;
    this->_msg = copy._msg;
}

Error &Error::operator = (const Error &src)
{
    if (this != &src)
    {
        this->_client = src._client;
        this->_msg = src._msg;
    }
    return *this;
}

Error::~Error() {};