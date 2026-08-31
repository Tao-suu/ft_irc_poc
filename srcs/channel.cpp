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
         _Operators.erase(std::find(_Operators.begin(), _Operators.end(), client));
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
    else
        MessageClient(client, "You need the operator privilege to use this commande");
}

void Channel::SetUserLimitMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if (!_ModeUserLimit)
        {
           std::cout << "The channel is now on User Limit Mode"<<std::endl;
            _ModeUserLimit = true;
        }
        else
            MessageClient(client, "The channel is already on User Limit Mode");
    }
    else
        MessageClient(client, "You need the operator privilege to use this commande");
}

//limit max ? protect from int max + 1 ? Accept long ?
void Channel::SetUserLimit(Client *client, unsigned int limit)
{
    if (IsAnOperator(client))
    {
        if (!_ModeUserLimit)
           MessageClient(client, "The channel is not on User Limit Mode");
        else
        {
            std::cout << "The channel has now a limit of " << limit << " users";
            _UserLimit = limit;
        }
    }
    else
        MessageClient(client, "You need the operator privilege to use this commande");
}

void Channel::RemoveUserLimitMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if (_ModeUserLimit)
        {
           std::cout << "The channel is not on User Limit Mode anymore"<<std::endl;
            _ModeUserLimit = false;
            _UserLimit = 0;
        }
        else
            MessageClient(client, "The channel is not on UserLimit Mode, you can remove it");
    }
    else
        MessageClient(client, "You need the operator privilege to use this commande");
}

void Channel::SetKeyMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if (!_ModeKey)
        {
           std::cout << "The channel is now on Key Mode"<<std::endl;
            _ModeKey = true;
        }
        else
            MessageClient(client, "The channel is already on Key Mode");
    }
    else
        MessageClient(client, "You need the operator privilege to use this commande");
}

//params for key (size, need letters and number ?)
void Channel::SetKey(Client *client, std::string key)
{
   if (IsAnOperator(client))
    {
        if (!_ModeKey)
           MessageClient(client, "The channel is not on User Limit Mode");
        else
        {
            std::cout << "The channel has now a key" << std::endl;
            _Key = key;
        }
    }
    else
        MessageClient(client, "You need the operator privilege to use this commande");
}

void Channel::RemoveKeyMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if (_ModeKey)
        {
           std::cout << "The channel is not on Key Mode anymore"<<std::endl;
            _ModeKey = false;
            _Key = "";
        }
        else
            MessageClient(client, "The channel is not on Key Mode, you can not remove it");
    }
    else
        MessageClient(client, "You need the operator privilege to use this commande");
}

void Channel::SetTopicMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if (!_ModeTopic)
        {
           std::cout << "The channel is now on Topic Mode"<<std::endl;
            _ModeTopic = true;
        }
        else
            MessageClient(client, "The channel is already on Topic Mode");
    }
    else
        MessageClient(client, "You need the operator privilege to use this commande");
}

//params for topic (size of the string)
void Channel::SetTopic(Client *client, std::string topic)
{
   if (IsAnOperator(client))
    {
        if (!_ModeTopic)
           MessageClient(client, "The channel is not on Topic Mode");
        else
        {
            std::cout << "The Topic of the channel is now "<< topic << std::endl;
            _Topic = topic;
        }
    }
    else
        MessageClient(client, "You need the operator privilege to use this commande");
}

void Channel::RemoveTopicMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if (_ModeTopic)
        {
           std::cout << "The channel is not on Topic Mode anymore"<<std::endl;
            _ModeTopic = false;
            _Topic = "";
        }
        else
            MessageClient(client, "The channel is not on Topic Mode, you can not remove it");
    }
    else
        MessageClient(client, "You need the operator privilege to use this commande");
}