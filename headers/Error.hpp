#pragma once

#include <client.hpp>

class Error
{
        public:
        Client _client;
        std::string _msg;

        Error(Client client, std::string);
        Error(const Error &copy);
        Error &operator = (const Error &src);
        ~Error();
};