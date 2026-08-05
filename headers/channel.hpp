#pragma once 

#include "client.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

class Channel
{
    private :
        std::string             _Name;

        std::vector<Client*>     _Clients;
        std::vector<Client*>     _Invitation;
        std::vector<Client*>     _Operators;
        unsigned int            _UserLimit;
        std::string             _Key;
        std::string             _Topic;
        //mode
        //bot

    public :
        Channel();
        Channel(std::string name);
        Channel(const Channel &copy);
        Channel &operator= (const Channel &src);
        ~Channel();

        std::string             getName() const;
        std::vector<Client*>     getListClient() const;
        std::vector<Client*>     getListInvitation() const;
         std::vector<Client*>    getOperators() const;
        unsigned int            getUserLimit() const;
        std::string             getKey() const;
        std::string             getTopic() const;

        void AddClient(Client *client);
        void RemoveClient(Client *client);
        bool IsClientOnTheList(Client *client) const;

        void AddClientInvitation(Client *client);
        void RemoveClientInvitation(Client *client);
        bool IsClientOnTheListInvitation(Client *client) const;

        void SetOperator(Client *client);
        void RemoveOperator(Client *client);
        bool IsAnOperator(Client *client);

        void SetUserLimit(unsigned int limit);
        void RemoveUserLimit();
        bool HasAnUserLimit();

        void SetKey(std::string key);
        void RemoveKey();
        bool HasAnKey();

        void SetTopic(std::string Topic);
        void RemoveTopic();

        //add remove has mode
        // add remove has mode
};