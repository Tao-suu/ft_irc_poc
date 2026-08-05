#include "channel.hpp"

Channel::Channel(){};

Channel::Channel(std::string name) : _Name(name) {}

Channel::Channel(const Channel &copy) {*this = copy;}

Channel &Channel::operator=(const Channel &src)
{
    if (this != &src)
    { *this = src; }
    return (*this);
}

Channel::~Channel(){};

std::string Channel::getName() const
{
    return(_Name);
}
std::vector<Client*> Channel::getListClient() const
{
    return(_Clients);
}
std::vector<Client*> Channel::getListInvitation() const
{
    return(_Invitation);
}
std::vector<Client*> Channel::getOperators() const
{
    return(_Operators);
}
unsigned int Channel::getUserLimit() const
{
    return(_UserLimit);
}
std::string Channel::getKey() const
{
    return(_Key);
}
std::string Channel::getTopic() const
{
    return(_Topic);
}

void Channel::AddClient(Client *client)
{
    if (_UserLimit != 0 && _Clients.size() >= _UserLimit)
         return ; //trow execption
    _Clients.push_back(client);

}
void Channel::RemoveClient(Client *client)
{
    if (!Channel::IsClientOnTheList(client))
        _Clients.erase(std::find(_Clients.begin(), _Clients.end(), client));
    if (!Channel::IsClientOnTheListInvitation(client))
        _Invitation.erase(std::find(_Clients.begin(), _Clients.end(), client));
    //be careful if client is the operator
}
bool Channel::IsClientOnTheList(Client *client) const
{
    if ((std::find(_Clients.begin(), _Clients.end(), client)) == _Clients.end())
        return false;
    return true;
}

void Channel::AddClientInvitation(Client *client)
{
    _Invitation.push_back(client);
}
void Channel::RemoveClientInvitation(Client *client)
{
    if (!Channel::IsClientOnTheListInvitation(client))
        _Invitation.erase(std::find(_Clients.begin(), _Clients.end(), client));
}

bool Channel::IsClientOnTheListInvitation(Client *client) const
{
     if ((std::find(_Invitation.begin(), _Invitation.end(), client)) ==_Invitation.end())
        return false;
    return true;
}

void Channel::SetOperator(Client *client)
{
    _Operators.push_back(client);
}
void Channel::RemoveOperator(Client *client)
{
    if (Channel::IsAnOperator(client))
        _Clients.erase(std::find(_Operators.begin(), _Operators.end(), client));
}

bool Channel::IsAnOperator(Client *client)
{
    if ((std::find(_Operators.begin(), _Operators.end(), client)) == _Operators.end())
        return false;
    return true;
}


void Channel::SetUserLimit(unsigned int limit)
{
    _UserLimit = limit;
    //mode
}

void Channel::RemoveUserLimit()
{
   if(Channel::HasAnUserLimit())
    _UserLimit = 0;
    //trow
    //mode
}

bool Channel::HasAnUserLimit()
{
    if (_UserLimit == 0)
        return false;
    return true;
}

void Channel::SetKey(std::string key)
{
    if (!Channel::HasAnKey())
        //mode;
    _Key = key;
}

void Channel::RemoveKey()
{
    if (!Channel::HasAnKey())
        return ; //error
    _Key = "";
    //mode
}

bool Channel::HasAnKey()
{
    if (_Key == "")
        return false;
    return true;
    // mode
}

void Channel::SetTopic(std::string Topic)
{
    _Topic = Topic;
}
void Channel::RemoveTopic()
{
    _Topic = "";
}