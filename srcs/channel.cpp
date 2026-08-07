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

void Channel::JoinChannel(Client *client)
{
    if (IsClientInChannel(client))
    {
        if (!_ModeInviteOnly)
        {
            if (_ModeUserLimit || _Clients.size() < _UserLimit)
                AddClient(client);
            throw UserLimitException();
        }
        else
        {
            if (Channel::IsClientOnTheListInvitation(client))
            {
                if (!_ModeUserLimit || _Clients.size() < _UserLimit)
                    AddClient(client);
                throw UserLimitException();
            }
            throw NotInvitationListException();
        }
    }
    throw IsAnUserException();
}

void Channel::AddClient(Client *client)
{
    _Clients.push_back(client);
    std::cout << client->GetUsername() << "join the channel" << std::endl;
}

void Channel::ExitChannel(Client *client)
{
    _Clients.erase(std::find(_Clients.begin(), _Clients.end(), client));
    std::cout << client->GetUsername() << "is removed to the channel" << std::endl;
    if (IsAnOperator(client))
        RemoveOperator(client);
}

void Channel::KickClient(Client *client, Client *user)
{
    if (IsAnOperator(client))
    {
        if (!IsClientOnTheListInvitation(user))
            throw NotAnUserException();
        ExitChannel(client);
    }
    //only see by the user
    throw NotAnOperatorException();
}

bool Channel::IsClientInChannel(Client *client) const
{
    if ((std::find(_Clients.begin(), _Clients.end(), client)) == _Clients.end())
        return false;
    return true;
}

void Channel::InvitClient(Client *client, Client *user)
{
    if (IsAnOperator(client))
    {
        if (IsClientOnTheListInvitation(user))
            throw IsAnUserException();
        _Invitation.push_back(client);
        throw InvitationSendException();
    }
    //only see by the client
    throw NotAnOperatorException();
}

void Channel::RemoveClientInvitation(Client *client, Client *user)
{
    if (IsAnOperator(client))
    {
        if (!IsClientOnTheListInvitation(user))
            throw NotAnUserException();
        _Invitation.erase(std::find(_Clients.begin(), _Clients.end(), client));
        throw RemoveFromInvitationException();
    }
    //only see by the client
    throw NotAnOperatorException();
}

bool Channel::IsClientOnTheListInvitation(Client *client) const
{
    if ((std::find(_Invitation.begin(), _Invitation.end(), client)) ==_Invitation.end())
        return false;
    return true;
}

void Channel::SetInviteOnlyMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if(_ModeInviteOnly)
            throw ModeOnException();
        _ModeInviteOnly = true;
    }
     //only see by the client
    throw NotAnOperatorException();
}

void Channel::GiveOperatorPrivilege(Client *client, Client *user)
{
    if (IsAnOperator(client))
    {
        
    }
     //only see by the client
    throw NotAnOperatorException();

    _Operators.push_back(client);
    std::cout << client->GetUsername() << "is now an operator" << std::endl;
}
void Channel::RemoveOperator(Client *client)
{
    _Operators.erase(std::find(_Operators.begin(), _Operators.end(), client));
    std::cout << client->GetUsername() << "is no longer an operator" << std::endl;
}

bool Channel::IsAnOperator(Client *client)
{
    if ((std::find(_Operators.begin(), _Operators.end(), client)) == _Operators.end())
        return false;
    return true;
}

void Channel::SetOperatorMode()
{
    _ModeOperator = true;
}


// void Channel::SetUserLimit(unsigned int limit)
// {
//     _UserLimit = limit;
//     std::cout << "The channel is now limited to " << limit << " user" << std::endl;
//     _ModeUserLimit = true;
// }

// void Channel::RemoveUserLimit()
// {
//     _UserLimit = 0;
//     std::cout << "The channel has no longer an user limit" << std::endl;
//     _ModeUserLimit = false;
// }

// bool Channel::HasAnUserLimit()
// {
//     if (_UserLimit == 0)
//         return false;
//     return true;
// }

// void Channel::SetKey(std::string key)
// {
//     _Key = key;
//     if (Channel::HasAnKey())
//         throw HasAnKeyException();
//     std::cout << "The channel has now a password" << std::endl;
//     _ModeKey = true;
// }

// void Channel::RemoveKey()
// {
//     _Key = "";
//     std::cout << "The password is removed" << std::endl;
//     _ModeKey = false;
// }

// bool Channel::HasAnKey()
// {
//     if (_Key == "")
//         return false;
//     return true;
// }

// void Channel::SetTopic(std::string topic)
// {
//     _Topic = topic;
//     std::cout << "The topic of the channel is"<< topic << std::endl;
//     _ModeTopic = true;
// }
// void Channel::RemoveTopic()
// {
//     _Topic = "";
//     std::cout << "The topic was removed" << std::endl;
//     _ModeTopic = false;
// }

// bool Channel::HasAnTopic()
// {
//     if (_Topic != "");
//         return false;
//     return true;
// }