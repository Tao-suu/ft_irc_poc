#pragma once 

#include "client.hpp"

#include <iostream>
#include <vector>

class Channel
{
    private :
        std::string             _Name;
        //adresse ?

        bool                    _HasATopic;
        std::string             _Topic;

        std::vector<Client>     _ListClients;
        std::vector<Client>     _ListInvitation;
        Client                  _Administrator;

        //std::vector<Mode>     _ListMode ?

};