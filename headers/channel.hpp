#pragma once 

#include "client.hpp"
#include <exception>
#include <iostream>
#include <vector>
#include <algorithm>

//what the operator mode ??

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

        void SetUserLimit(unsigned int limit);
        void RemoveUserLimit();
        bool HasAnUserLimit();

        void SetKey(std::string key);
        void RemoveKey();
        bool HasAnKey();

        void SetTopic(std::string Topic);
        void RemoveTopic();
        bool HasAnTopic();
};

// class UserLimitException : public std::exception
// {
//     public:
//         virtual const char * what() const throw() {return ("The user limit is reached on this channel, you can join it");}
// };