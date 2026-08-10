#pragma once 

#include "client.hpp"
#include <exception>
#include <iostream>
#include <vector>
#include <algorithm>

class Channel
{
    private :
        std::string             _Name;

        std::vector<Client*>     _Clients;
        std::vector<Client*>     _Invitations;
        std::vector<Client*>     _Operators;
        unsigned int            _UserLimit;
        std::string             _Key;
        std::string             _Topic;
        
        bool                    _ModeInviteOnly;
        bool                    _ModeTopic;
        bool                    _ModeKey;
        bool                    _ModeOperator;
        bool                    _ModeUserLimit;

    public :
        Channel();
        Channel(std::string name);
        Channel(const Channel &copy);
        Channel &operator= (const Channel &src);
        ~Channel();

        std::string             getName() const;
        std::vector<Client*>    getClients() const;
        std::vector<Client*>    getInvitedClients() const;
        std::vector<Client*>    getOperators() const;
        unsigned int            getUserLimit() const;
        std::string             getKey() const;
        std::string             getTopic() const;

        void JoinChannel(Client *client);
        void AddClient(Client *client);
        void ExitChannel(Client *client);
        void KickClient(Client *operators, Client *client);
        bool IsClientInChannel(Client *client) const;

        void InvitClient(Client *client, Client *user);
        void RemoveInvitedClient(Client *client, Client *user);
        bool IsClientInvited(Client *client) const;
        void SetInviteOnlyMode(Client *client);

        void GiveOperatorPrivilege(Client *client, Client *user);
        void RemoveOperator(Client *client, Client *user);
        bool IsAnOperator(Client *client);
        void SetOperatorMode(Client *client);

        void SetUserLimitMode(Client *client);
        void SetUserLimit(Client *client, unsigned int limit);
        void RemoveUserLimitMode(Client *client);

        void SetKeyMode(Client *client);
        void SetKey(Client *client, std::string key);
        void RemoveKeyMode(Client *client);
       
        void SetTopicMode(Client *client);
        void SetTopic(Client *client, std::string topic);
        void RemoveTopicMode(Client *client);
};

void MessageClient(Client *client, std::string message);
