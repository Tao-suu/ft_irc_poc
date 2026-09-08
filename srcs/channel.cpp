#include "channel.hpp"

/******************/
/* CANONICAL FORM */
/******************/

Channel::Channel(){};

Channel::Channel(std::string name, Server *server) : _Name(name), _Server(server) {}

Channel::Channel(const Channel &copy) {*this = copy;}

Channel &Channel::operator=(const Channel &src)
{
    if (this != &src)
    { *this = src; }
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
    if (_Mode & MODE_TOPIC)
    {
        MessageInClient(client, RPL_TOPIC(client->GetUsername(), _Name, _Topic));
        MessageInClient(client, RPL_TOPICWHOTIME(client->GetUsername(), _Name, _AutorTopic->GetUsername(), _TopicTime));
    }
}

void Channel::AddClient(Client *client)
{
    _Clients.push_back(client);
    if(IsClientInvited(client))
        _Invitations.erase(std::find(_Invitations.begin(), _Invitations.end(), client));
}

void Channel::ExitChannel(Client *client)
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
    ExitChannel(target);
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
    MessageInClient(client, RPL_INVITING(client->GetUsername(), client->GetNickname(), _Name));
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
        if(!(_Mode & MODE_INVITE_ONLY))
            _Mode = _Mode & MODE_USER_LIMIT;
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

void Channel::RemoveInviteOnlyMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if(_Mode & MODE_INVITE_ONLY)
            _Mode = _Mode ^ MODE_INVITE_ONLY;
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
}

void Channel::TakeOperatorPrivilege(Client *client, Client *target)
{
    if (!IsClientInChannel(client))
        throw Error(*client, ERR_NOTONCHANNEL(client->GetUsername(), _Name));
    if (!IsAnOperator(client))
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
    if (IsClientInChannel(target))
        throw Error(*client, ERR_USERONCHANNEL(client->GetUsername(), client->GetNickname(), _Name));
    _Operators.erase(std::find(_Operators.begin(), _Operators.end(), target));;
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

//limit - check if limit is valid before function (int positiv)
void Channel::SetUserLimit(Client *client, unsigned int limit)
{
    if (IsAnOperator(client))
    {
        if (_Mode & MODE_USER_LIMIT)
            _UserLimit = limit;
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

void Channel::SetUserLimitMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if (!(_Mode & MODE_USER_LIMIT))
            _Mode = _Mode & MODE_USER_LIMIT;
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

void Channel::RemoveUserLimitMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if (_Mode & MODE_USER_LIMIT)
        {
            _Mode = _Mode ^ MODE_USER_LIMIT;
            _UserLimit = 0;
        }
        // else msg can remove because not set
    }
    else
       throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

/*********************/
/*      KEY MODE    */
/********************/

//Key condition (10 cara / one letter / one digit ...)
void Channel::SetKey(Client *client, std::string key)
{
   if (IsAnOperator(client))
    {
        if (_Mode & MODE_KEY)
            _Key = key;
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

void Channel::SetKeyMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if (!(_Mode & MODE_KEY))
            _Mode = _Mode & MODE_KEY;
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

void Channel::RemoveKeyMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if (_Mode & MODE_KEY)
        {
            _Mode = _Mode ^ MODE_KEY;
            _Key = "";
        }
    }
    else
       throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}


/***********************/
/*      TOPIC MODE    */
/**********************/

//params for topic (size of the string)
void Channel::SetTopic(Client *client, std::string topic)
{
   if (IsAnOperator(client))
    {
        if (_Mode & MODE_TOPIC)
        {
            _Topic = topic;
            _AutorTopic = client;
            time_t timestamp;
            _TopicTime = time(&timestamp);
        }
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

void Channel::SetTopicMode(Client *client)
{
   if (IsAnOperator(client))
    {
        if (!(_Mode & MODE_TOPIC))
            _Mode = _Mode & MODE_TOPIC;
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));
}

void Channel::RemoveTopicMode(Client *client)
{
    if (IsAnOperator(client))
    {
        if (_Mode & MODE_TOPIC)
        {
            _Mode = _Mode ^ MODE_TOPIC;
            _Topic = "";
        }
    }
    else
       throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetUsername(), _Name));        
}

/*************************/
/*      MessageIn CLIENT    */
/*************************/

void MessageInClient(Client *client, std::string MessageIn)
{
	(void)client;
	(void)MessageIn;
}


/*************************/
/*        GETTER         */
/*************************/

std::string     Channel::getName() const { return this->_Name; }