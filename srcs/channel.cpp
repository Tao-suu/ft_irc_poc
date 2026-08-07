#include "channel.hpp"
#include "MessageError.hpp"

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
std::vector<Client*> Channel::getClients() const
{
    return(_Clients);
}
std::vector<Client*> Channel::getInvitedClients() const
{
    return(_Invitations);
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

// make it with switch and case ?
void Channel::JoinChannel(Client *client)
{
    if (IsClientInChannel(client))
    {
        if (!_ModeInviteOnly)
        {
            if (_ModeUserLimit || _Clients.size() < _UserLimit)
                AddClient(client);
            else       
                MessageClient(client, "You cannot join this channel, the user limit is reached");
        }
        else
        {
            if (IsClientInvited(client))
            {
                if (!_ModeUserLimit || _Clients.size() < _UserLimit)
                    AddClient(client);
                else
                    MessageClient(client, "You cannot join this channel, the user limit is reached");
            }
            else
                MessageClient(client, "You need an invitation to join thiss channel");
        }
    }
    else
        MessageClient(client, "You already are in this channel");
}

void Channel::AddClient(Client *client)
{
    _Clients.push_back(client);
    std::cout << client->GetUsername() << "join the channel" << std::endl;
    if(IsClientInvited(client))
        _Invitations.erase(std::find(_Invitations.begin(), _Invitations.end(), client));
}

void Channel::ExitChannel(Client *client)
{
    _Clients.erase(std::find(_Clients.begin(), _Clients.end(), client));
    MessageClient(client, "You left the channel");
    if (IsAnOperator(client))
        RemoveOperator(client);
    std::cout << client->GetUsername() << "left the channel" << std::endl;
}

void Channel::KickClient(Client *client, Client *user)
{
    if (IsAnOperator(client))
    {
        ExitChannel(client);
        MessageClient(user, "The user is kicked");
        MessageClient(client, "You have been kicked from the channel");
    }
    else
        MessageClient(user, "You need the operator privilege to use this commande");
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
        if(!IsClientInChannel(client))
        {
            if (IsClientInvited(user))
                MessageClient(user, "This user is already invited");
            else
            {
                _Invitations.push_back(client);
                MessageClient(user, "The invitation has been send");
                MessageClient(client, "You receive an invitation");
                // other function need to say witch channel
            }
        }
        else
            MessageClient(user, "The user is already in the channel");
    }
    else
        MessageClient(user, "You need the operator privilege to use this commande");
}

void Channel::RemoveInvitedClient(Client *client, Client *user)
{
    if (IsAnOperator(client))
    {
        if (IsClientInvited(user))
        {
            _Invitations.erase(std::find(_Invitations.begin(), _Invitations.end(), client));
            MessageClient(user, "The user is removed from the invitation list");
            //message to client that he is no longer on the list ?
        }
        else
            MessageClient(user, "The user is not on the list");
    }
    else
        MessageClient(user, "You need the operator privilege to use this commande");
}

bool Channel::IsClientInvited(Client *client) const
{
    if ((std::find(_Invitations.begin(), _Invitations.end(), client)) ==_Invitations.end())
        return false;
    return true;
}

void Channel::SetInviteOnlyMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if(!_ModeInviteOnly)
        {
            std::cout << "The channel is now on Invit Only Mode";
            _ModeInviteOnly = true;
        }
        else
            MessageClient(client, "The channel is already on Invit Only Mode");
    }
    MessageClient(client, "You need the operator privilege to use this commande");
}

void Channel::GiveOperatorPrivilege(Client *client, Client *user)
{
    if (IsAnOperator(user))
    {
        if(!IsClientInChannel(client))
            MessageClient(user, "This client is not in this channel");
        else
        {
            _Operators.push_back(client);
            std::cout << client->GetUsername() << "is now an operator" << std::endl;

        }
    }
    else
        MessageClient(user, "You need the operator privilege to use this commande");
}
void Channel::RemoveOperator(Client *client, Client *user)
{
    if (IsAnOperator(user))
    {
        if(!IsClientInChannel(client))
            MessageClient(user, "This client is not in this channel");
        else
        {
             _Operators.erase(std::find(_Operators.begin(), _Operators.end(), client));
            std::cout << client->GetUsername() << "is no longer an operator in this channel" << std::endl;  
        }
    }
    else
        MessageClient(user, "You need the operator privilege to use this commande");
}

bool Channel::IsAnOperator(Client *client)
{
    if ((std::find(_Operators.begin(), _Operators.end(), client)) == _Operators.end())
        return false;
    return true;
}

void Channel::SetOperatorMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if(!_ModeOperator)
        {
            std::cout << "The channel is now on Invit Only Mode";
            _ModeOperator = true;
        }
        else
            MessageClient(client, "The channel is already on Operator Mode");
    }
    MessageClient(client, "You need the operator privilege to use this commande");
}

void Channel::SetUserLimit(Client *client, unsigned int limit)
{
    if (IsAnOperator(client))
    {
        _UserLimit = limit;
        std::cout << "The channel is now limited to " << limit << " user" << std::endl;
        _ModeUserLimit = true;
    }
}

void Channel::RemoveUserLimit()
{
    _UserLimit = 0;
    std::cout << "The channel has no longer an user limit" << std::endl;
    _ModeUserLimit = false;
}

bool Channel::HasAnUserLimit()
{
    if (_UserLimit == 0)
        return false;
    return true;
}

void Channel::SetKey(std::string key)
{
    _Key = key;
    if (Channel::HasAnKey())
        throw HasAnKeyException();
    std::cout << "The channel has now a password" << std::endl;
    _ModeKey = true;
}

void Channel::RemoveKey()
{
    _Key = "";
    std::cout << "The password is removed" << std::endl;
    _ModeKey = false;
}

bool Channel::HasAnKey()
{
    if (_Key == "")
        return false;
    return true;
}

void Channel::SetTopic(std::string topic)
{
    _Topic = topic;
    std::cout << "The topic of the channel is"<< topic << std::endl;
    _ModeTopic = true;
}
void Channel::RemoveTopic()
{
    _Topic = "";
    std::cout << "The topic was removed" << std::endl;
    _ModeTopic = false;
}

bool Channel::HasAnTopic()
{
    if (_Topic != "");
        return false;
    return true;
}