#pragma once 

// to organize
# include "client.hpp"
# include <exception>
# include <iostream>
# include <vector>
# include <algorithm>
# include "Server.hpp"
# include "numerics.h"
# include "Error.hpp"
# include <ctime>

# define  MODE_INVITE_ONLY  (0 << 0)
# define  MODE_TOPIC        (1 << 0)
# define  MODE_KEY          (1 << 1)
# define  MODE_USER_LIMIT   (1 << 2)

class Channel
{
    private :
        std::string             _Name;
        std::vector<Client*>    _Clients;
        std::vector<Client*>    _Invitations;
        std::vector<Client*>    _Operators;
        unsigned int            _UserLimit;
        std::string             _Key;
        std::string             _Topic;
        char                    _Mode;
        Server*                 _Server;
        Client*                 _AutorTopic;
        std::string             _TopicTime;

    public :
        Channel();
        Channel(std::string name, Server* server);
        Channel(const Channel &copy);
        Channel &operator= (const Channel &src);
        ~Channel();

        void JoinChannel(Client *client, std::string key);
        void AddClient(Client *client);
        void ExitChannel(Client *client);
        void KickClient(Client *operators, Client *client);
        bool IsClientInChannel(Client *client) const;

        void InvitClient(Client *client, Client *target);
        bool IsClientInvited(Client *client) const;
        void SetInviteOnlyMode(Client *client);
        void RemoveInviteOnlyMode(Client *client);

        void GiveOperatorPrivilege(Client *client, Client *target);
        void TakeOperatorPrivilege(Client *client, Client *target);
        bool IsAnOperator(Client *client);

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



/******************/
/*      GETTER    */
/******************/ 
//   std::string             getName() const;
//         std::vector<Client*>    getClients() const;
//         std::vector<Client*>    getInvitedClients() const;
//         std::vector<Client*>    getOperators() const;
//         unsigned int            getUserLimit() const;
//         std::string             getKey() const;
//         std::string             getTopic() const;
// std::string Channel::getName() const
// {
//     return(_Name);
// }
// std::vector<Client*> Channel::getClients() const
// {
//     return(_Clients);
// }
// std::vector<Client*> Channel::getInvitedClients() const
// {
//     return(_Invitations);
// }
// std::vector<Client*> Channel::getOperators() const
// {
//     return(_Operators);
// }
// unsigned int Channel::getUserLimit() const
// {
//     return(_UserLimit);
// }
// std::string Channel::getKey() const
// {
//     return(_Key);
// }
// std::string Channel::getTopic() const
// {
//     return(_Topic);
// }

