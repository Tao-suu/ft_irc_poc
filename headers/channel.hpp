#pragma once 

#include "client.hpp"
#include <exception>
#include <iostream>
#include <vector>
#include <algorithm>

//when message for channel and for user
//remove 

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
        std::vector<Client*>    getListClient() const;
        std::vector<Client*>    getListInvitation() const;
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
        void RemoveClientInvitation(Client *client, Client *user);
        bool IsClientOnTheListInvitation(Client *client) const;
        void SetInviteOnlyMode(Client *client);

        void GiveOperatorPrivilege(Client *client, Client *user);
        void RemoveOperator(Client *client);
        bool IsAnOperator(Client *client);
        void SetOperatorMode();

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

class UserLimitException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("The user limit is reached on this channel, you can join it");}
};

class NotAnUserException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("The user is not a member of this channel, you cannot execute this command");}
};

class NotInvitationListException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("You need an invitation to join this channel");}
};

class IsAnUserException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("The user is already a member of this channel");}
};

class InvitationSendException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("The invitation is send");}
};

class RemoveFromInvitationException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("The user is removed to the invitation list");}
};

class NotAnOperatorException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("You need to be an operator to execute this command");}
};

class ModeOnException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("The mode is already actived");}
};

class NoUserLimitException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("There is no user limit to remove");}
};

class HasAnKeyException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("The passord is modified");}
};

class NoKeyException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("There is no passord to remove");}
};

class NoTopicException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("There is no topic to remove");}
};