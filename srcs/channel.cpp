#include "channel.hpp"

/******************/
/* CANONICAL FORM */
/******************/

Channel::Channel(): _Name(""), _UserLimit(0), _Mode(0), _Server(0), _AutorTopic(0) {};

Channel::Channel(std::string name, Server *server) : _Name(name), _UserLimit(0), _Mode(0), _Server(server), _AutorTopic(0) {}

Channel::Channel(const Channel &copy) {*this = copy;}

Channel &Channel::operator=(const Channel &src)
{
    if (this != &src)
    {
        _Name = src._Name;
        _Clients = src._Clients;
        _Invitations = src._Invitations;
        _Operators = src._Operators;
        _UserLimit = src._UserLimit;
        _Key = src._Key;
        _Topic = src._Topic;
        _Mode = src._Mode;
        _Server = src._Server;
        _AutorTopic = src._AutorTopic;
        _TopicTime = src._TopicTime;
    }
    return (*this);
}

Channel::~Channel(){};

/*************************/
/*     MANAGE CLIENTS    */
/*************************/

void Channel::JoinChannel(Client *client, std::string key)
{
    if (_Mode & MODE_KEY && _Key != key)
        throw Error(*client, ERR_BADCHANNELKEY(client->GetUsername(), _Name));
    if (_Mode & MODE_USER_LIMIT && _Clients.size() >= _UserLimit)
        throw Error(*client, ERR_CHANNELISFULL(client->GetUsername(), _Name));
    if(_Mode & MODE_INVITE_ONLY && !IsClientInvited(client))
        throw Error(*client, ERR_INVITEONLYCHAN(client->GetUsername(), _Name));
    AddClient(client);
    MessageListClients(_Clients, DFL_JOIN(client->GetUsername(), _Name));
    if (_Mode & MODE_TOPIC)
    {
        MessageClient(client, RPL_TOPIC(client->GetUsername(), _Name, _Topic));
        MessageClient(client, RPL_TOPICWHOTIME(client->GetUsername(), _Name, _AutorTopic->GetUsername(), _TopicTime));
    }
}

void Channel::AddClient(Client *client)
{
    _Clients.push_back(client);
    if(IsClientInvited(client))
        _Invitations.erase(std::find(_Invitations.begin(), _Invitations.end(), client));
}

void Channel::ExitClient(Client *client)
{
    _Clients.erase(std::find(_Clients.begin(), _Clients.end(), client));
    if (IsAnOperator(client))
        _Operators.erase(std::find(_Operators.begin(), _Operators.end(), client));
}

void Channel::KickClient(Client *client, Client *target)
{
    if (!IsAnOperator(client))
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
    if (!IsClientInChannel(target))
        throw Error(*client, ERR_USERNOTINCHANNEL(client->GetUsername(), client->GetNickname(), _Name));
    if (!IsClientInChannel(client))
        throw Error(*client, ERR_NOTONCHANNEL(client->GetUsername(), _Name));
    ExitClient(target);
    MessageClient(target, DFL_KICK(client->GetUsername(), target->GetUsername(), _Name));
    MessageClient(client, DFL_KICK(client->GetUsername(), target->GetUsername(), _Name));
}

bool Channel::IsClientInChannel(Client *client) const
{
    if ((std::find(_Clients.begin(), _Clients.end(), client)) == _Clients.end())
        return false;
    return true;
}

/***********************/
/*      INVITE MODE    */
/***********************/

void Channel::InvitClient(Client *client, Client *target)
{
    if (!IsClientInChannel(client))
        throw Error(*client, ERR_NOTONCHANNEL(client->GetUsername(), _Name));
    if (!IsAnOperator(client))
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
    if (IsClientInChannel(target))
        throw Error(*client, ERR_USERONCHANNEL(client->GetUsername(), client->GetNickname(), _Name));
    _Invitations.push_back(target);
    MessageClient(client, RPL_INVITING(client->GetUsername(), client->GetNickname(), _Name));
    MessageClient(target, DFL_INVITE(client->GetUsername(), target->GetUsername(), _Name));
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
         _Mode = _Mode & MODE_USER_LIMIT;
        MessageListClients(_Clients, DFL_SETINVITEMODE(client->GetUsername(), _Name));
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

void Channel::RemoveInviteOnlyMode(Client *client)
{
    if (IsAnOperator(client))
    {
        _Mode = _Mode ^ MODE_INVITE_ONLY;
        MessageListClients(_Clients, DFL_REMOVEINVITEMODE(client->GetUsername(), _Name));
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

/*************************/
/*      OPERATOR MODE    */
/*************************/

void Channel::GiveOperatorPrivilege(Client *client, Client *target)
{
    if (!IsClientInChannel(client))
        throw Error(*client, ERR_NOTONCHANNEL(client->GetUsername(), _Name));
    if (!IsAnOperator(client))
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
    if (IsClientInChannel(target))
        throw Error(*client, ERR_USERONCHANNEL(client->GetUsername(), client->GetNickname(), _Name));
    _Operators.push_back(target);
   MessageListClients(_Clients, DFL_TAKEOPERATORPRIVILEGE(client->GetUsername(), client->GetUsername(), _Name));
}

void Channel::TakeOperatorPrivilege(Client *client, Client *target)
{
    if (!IsClientInChannel(client))
        throw Error(*client, ERR_NOTONCHANNEL(client->GetUsername(), _Name));
    if (!IsAnOperator(client))
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
    if (IsClientInChannel(target))
        throw Error(*client, ERR_USERONCHANNEL(client->GetUsername(), client->GetNickname(), _Name));
    _Operators.erase(std::find(_Operators.begin(), _Operators.end(), target));
    MessageListClients(_Clients, DFL_GIVEOPERATORPRIVILEGE(client->GetUsername(), client->GetUsername(), _Name));
}

bool Channel::IsAnOperator(Client *client)
{
    if ((std::find(_Operators.begin(), _Operators.end(), client)) == _Operators.end())
        return false;
    return true;
}

/*************************/
/*      USER MODE        */
/*************************/

void Channel::SetUserLimit(Client *client, unsigned int limit)
{
    std::ostringstream ss;
    ss << limit;
    if (IsAnOperator(client))
    {
        _Mode = _Mode & MODE_USER_LIMIT;
        _UserLimit = limit;
        MessageListClients(_Clients, DFL_SETUSERLIMIT(client->GetUsername(), ss.str(), _Name));
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

void Channel::RemoveUserLimit(Client *client)
{
    if (IsAnOperator(client))
    {
        _Mode = _Mode ^ MODE_USER_LIMIT;
        _UserLimit = 0;
        MessageListClients(_Clients, DFL_REMOVEUSERLIMITMODE(client->GetUsername(), _Name));
    }
    else
       throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

/*********************/
/*      KEY MODE    */
/********************/

void Channel::SetKey(Client *client, std::string key)
{
   if (IsAnOperator(client))
    {
        _Key = key;
        _Mode = _Mode & MODE_KEY;
        MessageListClients(_Clients, DFL_SETKEY(client->GetUsername(), _Name));
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

void Channel::RemoveKey(Client *client)
{
    if (IsAnOperator(client))
    {
        _Mode = _Mode ^ MODE_KEY;
        _Key = "";
        MessageListClients(_Clients, DFL_REMOVEKEYMODE(client->GetUsername(), _Name));
    }
    else
       throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}


/***********************/
/*      TOPIC MODE    */
/**********************/

void Channel::SetTopic(Client *client, std::string topic)
{
   if (IsAnOperator(client))
    {
        _Mode = _Mode & MODE_TOPIC;
        _Topic = topic;
        _AutorTopic = client;
        time_t timestamp;
        _TopicTime = time(&timestamp);
        MessageListClients(_Clients, DFL_SETTOPIC(client->GetUsername(), topic, _Name));
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

void Channel::RemoveTopic(Client *client)
{
    if (IsAnOperator(client))
    {
        _Mode = _Mode ^ MODE_TOPIC;
        _Topic = "";
        MessageListClients(_Clients, DFL_REMOVETOPICMODE(client->GetUsername(), _Name));
    }
    else
       throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));        
}

/*************************/
/*      MESSAGE CLIENT    */
/*************************/

void    Channel::MessageClient(Client *client, std::string message)
{
	if (!_Server || !client) return ;

    MessageOut m(message + SEPARATOR);
    m.addTarget(client->GetFd());
    _Server->push_message(m);
}

void Channel::MessageListClients(std::vector<Client*> clients, std::string message)
{
    if (!_Server) return ;
    MessageOut m(message + SEPARATOR);
    for (size_t i = 0; i < clients.size(); i++)
        if (clients[i])
            m.addTarget(clients[i]->GetFd());
    _Server->push_message(m);
}

/*************************/
/*        GETTER         */
/*************************/

std::string             Channel::getName() const { return this->_Name; }
std::vector<Client*>    Channel::getClients() const { return this->_Clients; }