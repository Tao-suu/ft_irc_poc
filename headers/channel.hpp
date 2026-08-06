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

        void AddClient(Client *client);
        void RemoveClient(Client *client);
        bool IsClientOnTheList(Client *client) const;

        void AddClientInvitation(Client *client);
        void RemoveClientInvitation(Client *client);
        bool IsClientOnTheListInvitation(Client *client) const;
        void SetModeInviteOnlyMode();

        void SetOperator(Client *client);
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
        virtual const char * what() const throw() {return ("The user limit is reached on this channel, it can be added");}
};

class NotAnUserException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("The user is not a member of this channel, it can be removed");}
};

class NotInvitationListException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("The user is not on the invitation list, it can be removed");}
};

class IsAnUserException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("The user is already a member of this channel");}
};

class NotAnOperatorException : public std::exception
{
    public:
        virtual const char * what() const throw() {return ("The user is not an operator, it can be removed");}
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