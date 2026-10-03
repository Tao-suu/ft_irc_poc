#include "channel.hpp"

/******************/
/* CANONICAL FORM */
/******************/

Channel::Channel(): _Name(""), _UserLimit(0), _Mode(0), _Server(0), _AutorTopic(0) {}

Channel::Channel(std::string name, Server *server, Client* creator): _Name(name), _UserLimit(0), _Mode(0), _Server(server), _AutorTopic(0) { AddClient(creator); _Operators.push_back(creator); }

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
 
Channel::~Channel(){}

/*************************/
/*     MANAGE CLIENTS    */
/*************************/

void Channel::JoinChannel(Client *client, std::string key)
{
    if (_Mode & MODE_KEY && _Key != key)
        throw Error(*client, ERR_BADCHANNELKEY(client->GetNickname(), _Name));
    if (_Mode & MODE_USER_LIMIT && _Clients.size() >= _UserLimit)
        throw Error(*client, ERR_CHANNELISFULL(client->GetNickname(), _Name));
    if(_Mode & MODE_INVITE_ONLY && !IsClientInvited(client))
        throw Error(*client, ERR_INVITEONLYCHAN(client->GetNickname(), _Name));
    AddClient(client);
    if (_Clients.size() == 1) _Operators.push_back(_Clients.back());
    // MessageListClients(_Clients, DFL_JOIN(client->GetNickname(), _Name));
    MessageOut m0; m0.setMessage(PREFIX(client->GetNickname(), client->GetUsername(), client->GetIP()) + " JOIN " + _Name + SEPARATOR);
    for (size_t j = 0; j < _Clients.size(); j++) m0.addTarget(_Clients[j]->GetFd());
    _Server->push_message(m0);
    if (!_Topic.empty())
    {
        MessageClient(client, RPL_TOPIC(client->GetNickname(), _Name, _Topic));
        MessageClient(client, RPL_TOPICWHOTIME(client->GetNickname(), _Name, _AutorTopic.GetNickname(), _TopicTime));
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

void Channel::KickClient(Client *client, Client *target,  std::string comment)
{
    MessageListClients(getClients(), PREFIX(client->GetNickname(), client->GetUsername(), client->GetIP()) + " KICK " + _Name + " " + target->GetNickname() + " :" + comment);
    ExitClient(target);
}

void Channel::PartClient(Client *client, std::string reason)
{
    MessageClient(client, PREFIX(client->GetNickname(),client->GetUsername(), client->GetIP()) + " PART " + _Name + ' ' + reason);
    ExitClient(client);
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
    _Invitations.push_back(target);
    MessageClient(client, RPL_INVITING(client->GetNickname(), target->GetNickname(), _Name));
    MessageClient(target, PREFIX(client->GetNickname(), client->GetUsername(), client->GetIP()) + " INVITE " + target->GetNickname() + " " + _Name);
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
        _Mode |= MODE_INVITE_ONLY;
        MessageListClients(_Clients, DFL_SETINVITEMODE(client->GetNickname(), _Name));
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetNickname(), _Name));
}

void Channel::RemoveInviteOnlyMode(Client *client)
{
    if (IsAnOperator(client))
    {
        _Mode &= ~MODE_INVITE_ONLY;
        MessageListClients(_Clients, DFL_REMOVEINVITEMODE(client->GetNickname(), _Name));
    }
    else
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetNickname(), _Name));
}

/*************************/
/*      OPERATOR MODE    */
/*************************/

bool Channel::GiveOperatorPrivilege(Client *target)
{
    if (std::find(_Operators.begin(), _Operators.end(), target) != _Operators.end()) return false;
    _Operators.push_back(target);
    return true;
}

bool Channel::TakeOperatorPrivilege(Client *target)
{
    std::vector<Client *>::iterator cit = std::find(_Operators.begin(), _Operators.end(), target);
    if (cit == _Operators.end()) return false;
    _Operators.erase(cit);
    return true;
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

void Channel::SetUserLimit(unsigned int limit)
{
    _Mode |= MODE_USER_LIMIT;
    _UserLimit = limit;
}

void Channel::RemoveUserLimit()
{
    _Mode &= ~MODE_USER_LIMIT;
    _UserLimit = 0;
}

/*********************/
/*      KEY MODE    */
/********************/

void Channel::SetKey(std::string key)
{
    _Key = key;
    _Mode |= MODE_KEY;
}

void Channel::RemoveKey()
{
    _Mode &= ~MODE_KEY;
    _Key = "";
}


/***********************/
/*      TOPIC MODE    */
/**********************/

void Channel::SetTopicMode(Client *client) {
    if (!IsAnOperator(client))
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetNickname(), _Name));
    
    _Mode |= MODE_TOPIC;
}

void Channel::RemoveTopicMode(Client *client) {
    if (!IsAnOperator(client))
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetNickname(), _Name));

    _Mode &= ~MODE_TOPIC;
}

void Channel::SetTopic(Client *client, std::string topic)
{
    if (!IsAnOperator(client) && _Mode & MODE_TOPIC)
        throw Error(*client, ERR_CHANOPRIVSNEEDED(client->GetNickname(), _Name));
    _Topic = topic;
    _AutorTopic = *client;
    time_t timestamp;
    std::ostringstream oss; oss << time(&timestamp);
    _TopicTime = oss.str();
    MessageListClients(_Clients, DFL_SETTOPIC(client->GetNickname(), client->GetUsername(), client->GetIP(), topic, _Name));
}

void Channel::RemoveTopic(Client *client)
{
    (void)client;
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

std::string&                    Channel::getName() { return this->_Name; }
std::vector<Client*>&           Channel::getClients() { return this->_Clients; }
std::string&                    Channel::getTopic() { return this->_Topic; }
std::string                     Channel::getAuthorTopic() { return this->_AutorTopic.GetNickname(); }
std::string&                    Channel::getTopicTime() { return this->_TopicTime; }
std::vector<Client*>::iterator  Channel::getClientByNick(const std::string& name) {
    for (std::vector<Client*>::iterator it = _Clients.begin(); it != _Clients.end(); it++) {
        if (to_upper_string((*it)->GetNickname()) == to_upper_string(name)) return it;
    }
    return _Clients.end();
}
int                             Channel::getMode() { return _Mode; }
std::string                     Channel::getUserlimit() { std::ostringstream ss; ss << _UserLimit; std::string tmp = ss.str(); return tmp; }
std::string&                    Channel::getKey() { return _Key; }


/*************************/
/*         UTILS         */
/*************************/

std::string     Channel::get_namereply( void ) {
    std::stringstream   ss;
    
    for (size_t i = 0; i < _Clients.size(); i++) {
        ss << (IsAnOperator(_Clients[i]) ? "@" : "");
        ss << _Clients[i]->GetNickname();
        if (i + 1 < _Clients.size()) ss << " ";
    }
    return ss.str();
}


bool            Channel::is_valid_name(const std::string& name) {
    if (name[0] != '#' || name.size() <= 1) return false;
    for (size_t i = 0; i < name.size(); i++) {
        if (::isspace(name[i]) || name[i] == ',' || name[i] == '\x07') return false;
    }
    return true;
}

bool            Channel::bitMode(int bit, bool state) {
    bool bitstate = (_Mode & bit) != 0;
    if (state == bitstate) return false;

    if (state) _Mode |= bit;
    else _Mode &= ~bit;
    return true;
}