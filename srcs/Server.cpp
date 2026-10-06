#include "Server.hpp"
#include <string>


Server::Server( void ): port_(0), pass_("") {}
Server::Server( int port, std::string pass ): port_(port), pass_(pass) {}
Server::Server( const Server& o ): port_(o.port_), pass_(o.pass_), servFd_(o.servFd_), addr_(o.addr_) {}
Server& Server::operator=( const Server& o )
{
    if (this != &o)
    {
        port_ = o.port_;
        pass_ = o.pass_;
        servFd_ = o.servFd_;
        addr_ = o.addr_;
    }
    return *this;
}
Server::~Server() {}

void    Server::init( void )
{
    servFd_ = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    if (setsockopt(servFd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) throw Server::ServerException("Server initialisation: " + std::string(std::strerror(errno)));
    if (fcntl(servFd_, F_SETFL, O_NONBLOCK) == -1) throw Server::ServerException("Server initialision: " + std::string(std::strerror(errno)));

    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port_);
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(servFd_, (struct sockaddr*)&addr_, sizeof(addr_)) == -1) throw Server::ServerException("Server initialision: " + std::string(std::strerror(errno)));

    if (listen(servFd_, SOMAXCONN) == -1) throw Server::ServerException("Server initialision: " + std::string(std::strerror(errno)));
}

void    Server::run ( void )
{
    pollfd  fd0;
    fd0.fd = servFd_;
    fd0.events = POLLIN;
    fd0.revents = 0;

    pollfds_.push_back(fd0);

    while (running)
    {
        /*      Setup of what we want to look     */
        for (size_t i = 0; i < pollfds_.size(); i++) {
            if (pollfds_[i].fd == servFd_) { pollfds_[i].events = POLLIN; continue; }
            pollfds_[i].events = POLLIN;
            if (Clients_.find(pollfds_[i].fd) != Clients_.end() && !Clients_[pollfds_[i].fd]._out_buffer.empty()) 
                pollfds_[i].events |= POLLOUT;
        }

        /*      Wait and get fd info     */
        int ret = poll(&pollfds_[0], pollfds_.size(), -1);
        if (ret == -1)
        {
            if (errno == EINTR) continue;
            break;      // Fatal error, stop server
        }

        /*      Dispatch: read, write and/or quit    */
        size_t  n = pollfds_.size();
        for (size_t i = 0; i < n; i++)
        {
            if (pollfds_[i].revents == 0) continue;
            if (pollfds_[i].fd == servFd_) {
                if (pollfds_[i].revents & POLLIN) { acceptNewClient(); }
                continue ;
            }
            if (pollfds_[i].revents & POLLIN) { handleClientData(pollfds_[i].fd); }
            if (pollfds_[i].revents & POLLOUT) { handleClientWrite(pollfds_[i].fd); }   
            if (pollfds_[i].revents & (POLLHUP | POLLERR | POLLNVAL)) { toRemove_.push_back(pollfds_[i].fd); }   
        }

        /*      Remove clients and close fd      */
        for (size_t i = 0; i < toRemove_.size(); i++)
        {
            int fd = toRemove_[i];
            if (Clients_.find(fd) == Clients_.end()) continue ;
            Clients_[fd]._in_buffer.erase();
            
            std::vector<int> peers;
            for (std::vector<Channel>::iterator chan_it = Channels_.begin(); chan_it != Channels_.end();) {
                if (chan_it->IsClientInvited(&Clients_[fd])) chan_it->RemoveInvitation(&Clients_[fd]); 
                if (chan_it->IsClientInChannel(&Clients_[fd])) {
                    for (std::vector<Client *>::iterator cit = chan_it->getClients().begin(); cit != chan_it->getClients().end(); cit++) {
                        if ((*cit)->GetFd() != fd && std::find(peers.begin(), peers.end(), (*cit)->GetFd()) == peers.end()) peers.push_back((*cit)->GetFd());
                    }
                    chan_it->ExitClient(&Clients_[fd]);
                    if (chan_it->getClients().size() == 0) {
                        Channels_.erase(chan_it); continue;
                    }
                }
                chan_it++;
            }
            MessageOut m(PREFIX(Clients_[fd].GetNickname(), Clients_[fd].GetUsername(), Clients_[fd].GetIP()) + " QUIT :Salut mon pote !" + SEPARATOR, peers); push_message(m);

            Clients_.erase(fd);
            close(fd);
            std::cout << "client disconnected\tfd = " << fd << SEPARATOR;
            for (unsigned int j = 0; j < pollfds_.size(); j++)
            {
                if (pollfds_[j].fd == fd) {
                    pollfds_.erase(pollfds_.begin() + j); 
                    break;
                }
            }
        }
        toRemove_.clear();

        /*      append message in client buffer     */
        send_all();          
    }
    for (size_t i = 0; i < pollfds_.size(); i++) {
        close(pollfds_[i].fd);
    }
}

int                 Server::get_port( void ) const {return port_;}
const std::string&  Server::get_password( void ) const {return pass_;}
const std::string   Server::get_ip( void ) const { return std::string(inet_ntoa(addr_.sin_addr)); }
void                Server::push_message( const MessageOut& m ) { message_stack.push_front(m); }

std::ostream&   operator<<(std::ostream& os, const Server& s)
{
    os << "Serveur: port=" << s.get_port() << " | pass=" << s.get_password();
    return os;
}

void                Server::acceptNewClient( void )
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    int fd = accept(servFd_, (struct sockaddr*)&addr, &len);
    if (fd == -1) return;
    fcntl(fd, F_SETFL, O_NONBLOCK);

    pollfd new_fd;
    new_fd.fd = fd;
    new_fd.revents = 0;
    new_fd.events = POLLIN;
    pollfds_.push_back(new_fd);

    Clients_[fd] = Client(fd);
    Clients_[fd].SetIpAdd(std::string(inet_ntoa(addr.sin_addr)));
	if (pass_.empty())
		Clients_[fd].pass_ok = true;
    std::cout << "new client fd = " << fd << "\taddr_ = " << Clients_[fd].GetIP() << SEPARATOR;
}

void                Server::handleClientData( int fd )
{
    char    buffer[2048];
    std::memset(buffer, 0, 2048);

    ssize_t bytes = recv(fd, buffer, 2048, 0);
    if (bytes <= 0) {toRemove_.push_back(fd); return ;}
    if (bytes > 0)  Clients_[fd]._in_buffer.append(buffer, bytes); 

    size_t  pos;
    while ((pos = Clients_[fd]._in_buffer.find(SEPARATOR)) != std::string::npos)
    {
        std::string line = Clients_[fd]._in_buffer.substr(0, pos);
        Clients_[fd]._in_buffer = Clients_[fd]._in_buffer.substr(pos + SEPARATOR.size(), Clients_[fd]._in_buffer.size() - (pos + SEPARATOR.size()));
        
        if (DEBUG_IN) std::cout << "\x1b[31;1m<- client fd(" << fd << ") : " << line << SEPARATOR << "\x1b[0m";
		
        if (!cv.validateContent(line)) continue ;
		MessageIn	msg = cv.parseContent(line);
		exec(msg, Clients_[fd]);
        if (std::find(toRemove_.begin(), toRemove_.end(), fd) != toRemove_.end()) break;
    }
}
void                Server::handleClientWrite( int fd ) {
    if (std::find(toRemove_.begin(), toRemove_.end(), fd) != toRemove_.end()) return ; 
    std::string& out = Clients_[fd]._out_buffer;
    if (out.empty()) return;
    ssize_t n = ::send(fd, out.c_str(), out.size(), 0);

    if (DEBUG_OUT) std::cout << "\x1b[32;1m-> client fd(" << fd << ") : " << out << "\x1b[0m";

    if (n <= 0) { toRemove_.push_back(fd); return; }
    out.erase(0, n);
}


void				Server::exec(MessageIn &msg, Client& sender)
{
    std::string cmdName = to_upper_string(msg.cmdName);
	try {
		if (cmdName == "PASS") pass(msg, sender);
		else if (cmdName == "NICK") nick(msg, sender);
		else if (cmdName == "USER") user(msg, sender);
        else if (cmdName == "PING") ping(msg, sender);
        else if (cmdName == "JOIN") join(msg, sender);
        else if (cmdName == "QUIT") toRemove_.push_back(sender.GetFd());
        else if (cmdName == "PRIVMSG") privmsg(msg, sender);
        else if (cmdName == "TOPIC") topic(msg, sender);
        else if (cmdName == "LIST") list(msg, sender);
        else if (cmdName == "PART") part(msg, sender);
        else if (cmdName == "MODE") mode(msg, sender);
        else if (cmdName == "INVITE") invite(msg, sender);
        else if (cmdName == "KICK") kick(msg, sender);
        else if (cmdName == "CAP") {}
		else throw Error(sender, ERR_UNKNOWCOMMAND((sender.GetNickname().empty() ? "*" : sender.GetNickname()), msg.cmdName));
	} catch (Error &e) {
        e._msg += SEPARATOR;
        MessageOut  err(e._msg); err.addTarget(e._client.GetFd());
		message_stack.push_front(err);
    }
}

void				Server::sendWelcome(Client &cl)
{
    if (!cl.pass_ok) {
        std::vector<int> targets; targets.push_back(cl.GetFd());
        message_stack.push_front(MessageOut(ERR_PASSWDMISMATCH("*") + SEPARATOR, targets));
        cl.SetNickname(""); cl.SetRealname(""); cl.SetUsername(""); cl.nick_ok = false; cl.pass_done = false; cl.pass_ok = false; cl.user_ok = false;
        return ;
    }
        
	cl.registered = true;
    std::time_t datetime = std::time(NULL);
    std::string stime = asctime(std::localtime(&datetime)); stime = stime.substr(0, stime.size() - 1);
    std::string     message = RPL_WELCOME(cl.GetNickname()) + "\r\n" +
                            RPL_YOURHOST(cl.GetNickname(), PROGRAM_NAME, VERSION) + "\r\n" +
                            RPL_CREATED(cl.GetNickname(), stime) + "\r\n" + 
                            RPL_MYINFO(cl.GetNickname(), PROGRAM_NAME, VERSION, MODE) + "\r\n";
    std::vector<int>    targets; targets.push_back(cl.GetFd());
    message_stack.push_front(MessageOut(message, targets));                     
}

void                Server::broadcastToPeer( Client& cl, const std::string& message ) {
    MessageOut  m(message + SEPARATOR);

    m.addTarget(cl.GetFd());
    for (std::vector<Channel>::iterator it = Channels_.begin(); it != Channels_.end(); it++) {
        if (!it->IsClientInChannel(&cl)) continue ;
        std::vector<Client*> members = it->getClients();
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i]) m.addTarget(members[i]->GetFd());
        }
    }
    push_message(m);
}

Server::ServerException::ServerException( const std::string& message ): message_(message) {}
const char* Server::ServerException::what() const throw() {return message_.c_str();}
Server::ServerException::~ServerException() throw() {}


/*******************************/
/* CHANNEL AND CLIENT FUNCTION */
/*******************************/

bool 				Server::is_nickname_exist(std::string nick) {
	for (std::map<int, Client>::iterator i = Clients_.begin(); i != Clients_.end(); i++)
		if (to_upper_string((*i).second.GetNickname()) == to_upper_string(nick)) return true;
	return false; 
}
bool                Server::is_channel_exist(std::string name) {
    for (std::vector<Channel>::iterator it = Channels_.begin(); it != Channels_.end(); it++) {
        if (to_upper_string((*it).getName()) == to_upper_string(name)) return true;
    }
    return false;
}
std::vector<Channel>::iterator Server::get_channel(std::string name) {
    for (std::vector<Channel>::iterator it = Channels_.begin(); it != Channels_.end(); it++) {
        if (to_upper_string((*it).getName()) == to_upper_string(name)) return it;
    }
    return Channels_.end();
}

std::map<int, Client>::iterator Server::get_client_by_nick(std::string nick) {
    for (std::map<int, Client>::iterator it = Clients_.begin(); it != Clients_.end(); it++) {
        if (!it->second.registered) continue ;
        if (to_upper_string(it->second.GetNickname()) == to_upper_string(nick)) return it;
    }
    return Clients_.end();
}

void                Server::send_all( void ) {
    while (!message_stack.empty()) {
        MessageOut& message = message_stack.back();
        std::vector<int>    targets = message.getTargets();
        for (size_t i = 0; i < targets.size(); i++) {
            std::map<int, Client>::iterator it = Clients_.find(targets[i]);
            if (it != Clients_.end())
                it->second._out_buffer += message.getMessage();
        }
        message_stack.pop_back();
    }
}

/****************************/
/*        IRC COMMAND       */
/****************************/

void				Server::pass(MessageIn &msg, Client& cl) {
    /* ERR MANAGE */
    if (msg.args.size() < 1 || msg.args[0].size() < 1) throw Error(cl, ERR_NEEDMOREPARAMS((cl.GetNickname().empty() ? "*" : cl.GetNickname()), "PASS"));
	if (cl.registered) throw Error(cl, ERR_ALREADYREGISTERED((cl.GetNickname().empty() ? "*" : cl.GetNickname())));
    
    cl.pass_done = true;
	if (msg.args[0][0] != this->pass_)
        cl.pass_ok = false;
    else
        cl.pass_ok = true;
    /* COMMAND CORE */
    if (cl.user_ok && cl.nick_ok) sendWelcome(cl);
}

void				Server::nick(MessageIn &msg, Client& cl) {
	/* ERR MANAGE */
	if (msg.args.size() < 1 || msg.args[0].size() < 1)
		throw Error(cl, ERR_NONICKNAMEGIVEN((cl.GetNickname().empty() ? "*" : cl.GetNickname())));
	else if (msg.args[0][0].find(':') != std::string::npos || msg.args[0][0].find(' ') != std::string::npos)
		throw Error(cl, ERR_ERRONEUSNICKNAME((cl.GetNickname().empty() ? "*" : cl.GetNickname()), msg.args[0][0]));
	else if (is_nickname_exist(msg.args[0][0]) && to_upper_string(msg.args[0][0]) != to_upper_string(cl.GetNickname()))
		throw Error(cl, ERR_NICKNAMEINUSE((cl.GetNickname().empty() ? "*" : cl.GetNickname()), msg.args[0][0]));
	
    /* COMMAND CORE */
    std::string oldNick = cl.GetNickname();
    cl.SetNickname(msg.args[0][0]); 
	if (cl.registered) {    
        broadcastToPeer(cl, MSG_NICK(PREFIX(oldNick, cl.GetUsername(), cl.GetIP()), msg.args[0][0]));
	} else { cl.nick_ok = true; }
	if (!cl.registered && cl.pass_done && cl.user_ok) 
		sendWelcome(cl);
}

void				Server::user(MessageIn &msg, Client& cl) {
	/* ERR MANAGE */
	if (msg.args.size() < 4)
	    throw Error(cl, ERR_NEEDMOREPARAMS(cl.GetNickname(), "USER"));
	if (cl.registered)
		throw Error(cl, ERR_ALREADYREGISTERED((cl.GetNickname().empty() ? "*" : cl.GetNickname())));
        
    /* COMMAND CORE */
    cl.SetRealname(msg.args[3][0]);
    cl.SetUsername(msg.args[0][0]);
    cl.user_ok = true;
    if (cl.pass_done && cl.nick_ok) sendWelcome(cl); 
}

void            Server::ping(MessageIn &msg, Client &cl) {
    if (msg.args.empty() || msg.args[0].empty()) 
	    throw Error(cl, ERR_NEEDMOREPARAMS((cl.GetNickname().empty() ? "*" : cl.GetNickname()), "PING"));
    
    MessageOut  m;
    m.addTarget(cl.GetFd());
    std::string s; for (size_t i = 0; i < msg.args[0].size(); i++) {
        s += msg.args[0][i];
        if (i + 1 < msg.args[0].size()) s += ',';
    }
    m.setMessage("PONG ft_irc :" + s + SEPARATOR);
    this->push_message(m);
}

void                Server::join(MessageIn& msg, Client& cl) {
    if (msg.args.empty() || msg.args[0].empty())
        throw Error(cl, ERR_NEEDMOREPARAMS((cl.GetNickname().empty() ? "*" : cl.GetNickname()), "JOIN"));
    if (!cl.registered)
            throw Error(cl, ERR_NOTREGISTERED((cl.GetNickname().empty() ? "*" : cl.GetNickname())));

    for (size_t i = 0; i < msg.args[0].size(); i++) {
        try {
            if (!Channel::is_valid_name(msg.args[0][i])) throw Error(cl, ERR_BADCHANNELMASK(cl.GetNickname(), msg.args[0][i]));
            std::vector<Channel>::iterator  chan_it = get_channel(msg.args[0][i]);
            if (chan_it == Channels_.end()) {
                Channel chan(msg.args[0][i], this, &cl);
                Channels_.push_back(chan);
                MessageOut m0; m0.addTarget(cl.GetFd()); m0.setMessage(PREFIX(cl.GetNickname(), cl.GetUsername(), cl.GetIP()) + " JOIN " + msg.args[0][i] + SEPARATOR); push_message(m0);
                MessageOut m1; m1.addTarget(cl.GetFd()); m1.setMessage(std::string(":ft_irc 353 ") + cl.GetNickname() + " = " + chan.getName() + " :" + chan.get_namereply() + SEPARATOR); push_message(m1); 
                MessageOut m2; m2.addTarget(cl.GetFd()); m2.setMessage(RPL_ENDOFNAMES(cl.GetNickname(), chan.getName()) + SEPARATOR); push_message(m2);
            } else {
                if (chan_it->IsClientInChannel(&cl)) continue;
                chan_it->JoinChannel(&cl, msg.args.size() > 1 ? (i < msg.args[1].size() ? msg.args[1][i] : "") : "");
                MessageOut m3; m3.addTarget(cl.GetFd()); m3.setMessage(std::string(":ft_irc 353 ") + cl.GetNickname() + " = " + chan_it->getName() + " :" + chan_it->get_namereply() + SEPARATOR); push_message(m3); 
                MessageOut m4; m4.addTarget(cl.GetFd()); m4.setMessage(RPL_ENDOFNAMES(cl.GetNickname(), chan_it->getName()) + SEPARATOR); push_message(m4); 
            }
        }
        catch(const Error& e) {
            MessageOut  m; m.addTarget(e._client.GetFd()); m.setMessage(e._msg + SEPARATOR);
            this->push_message(m);
        }
    }
}

void            Server::privmsg(MessageIn& msg, Client& cl) {
    if (!cl.registered)
        throw Error(cl, ERR_NOTREGISTERED((cl.GetNickname().empty() ? "*" : cl.GetNickname())));
    if (msg.args.empty() || msg.args[0].empty())
        throw Error(cl, ERR_NORECIPIENT(cl.GetNickname(), "PRIVMSG"));
    if (msg.args.size() < 2 || msg.args[1].empty() || msg.args[1][0].empty())
        throw Error(cl, ERR_NOTEXTTOSEND(cl.GetNickname()));

    for (size_t i = 0; i < msg.args[0].size(); i++) {
        try {
            if (msg.args[0][i][0] == '&' || msg.args[0][i][0] == '#') {
                std::vector<Channel>::iterator chan_it = get_channel(msg.args[0][i]);
                if (chan_it == Channels_.end()) throw Error(cl, ERR_NOSUCHNICK(cl.GetNickname(), msg.args[0][i]));
                if (!chan_it->IsClientInChannel(&cl)) throw Error(cl, ERR_CANNOTSENDTOCHAN(cl.GetNickname(), chan_it->getName()));
                std::string mtext;
                for (size_t i = 0; i < msg.args[1].size(); i++) {
                    mtext += msg.args[1][i];
                    if (i + 1 < msg.args[1].size()) mtext += ',';
                }
                MessageOut m; m.setMessage(PREFIX(cl.GetNickname(), cl.GetUsername(), cl.GetIP()) + " PRIVMSG " + chan_it->getName() + " :" + mtext + SEPARATOR); for (std::vector<Client*>::iterator cit = chan_it->getClients().begin(); cit != chan_it->getClients().end(); cit++) {
                    if ((*(cit))->GetFd() != cl.GetFd()) m.addTarget((*(cit))->GetFd());
                }
                push_message(m);
            } else {
                std::map<int, Client>::iterator cit = get_client_by_nick(msg.args[0][i]);
                if (cit == Clients_.end()) throw Error(cl, ERR_NOSUCHNICK(cl.GetNickname(), msg.args[0][i]));
                MessageOut m; m.setMessage(PREFIX(cl.GetNickname(), cl.GetUsername(), cl.GetIP()) + " PRIVMSG " + cit->second.GetNickname() + " :" + msg.args[1][0] + SEPARATOR);
                m.addTarget(cit->second.GetFd()); push_message(m);
            }
        } catch(const Error& e) {
            MessageOut  m; m.addTarget(e._client.GetFd()); m.setMessage(e._msg + SEPARATOR);
            this->push_message(m);
        }
    }
}

void            Server::topic(MessageIn& msg, Client& cl) {
    if (!cl.registered)
        throw Error(cl, ERR_NOTREGISTERED(cl.GetNickname()));
    if (msg.args.size() < 1)
        throw Error(cl, ERR_NEEDMOREPARAMS(cl.GetNickname(), "TOPIC"));
    
    std::vector<Channel>::iterator  chan_it = get_channel(msg.args[0][0]);
    if (chan_it == Channels_.end())
        throw Error(cl, ERR_NOSUCHCHANNEL(cl.GetNickname(), msg.args[0][0]));
    if (!chan_it->IsClientInChannel(&cl))
        throw Error(cl, ERR_NOTONCHANNEL(cl.GetNickname(), msg.args[0][0]));

    std::cout << msg.args.size() << SEPARATOR;
    if (msg.args.size() == 1)
    {
        MessageOut m; m.addTarget(cl.GetFd()); m.setMessage(chan_it->getTopic().empty() ? 
        RPL_NOTOPIC(cl.GetNickname(), chan_it->getName()) + SEPARATOR : 
        RPL_TOPIC(cl.GetNickname(), chan_it->getName(), chan_it->getTopic()) + SEPARATOR + RPL_TOPICWHOTIME(cl.GetNickname(), chan_it->getName(), chan_it->getAuthorTopic(), chan_it->getTopicTime()) + SEPARATOR 
        );
        push_message(m);
        return ;
    }
    chan_it->SetTopic(&cl, msg.args[1][0]);
}

void            Server::list(MessageIn& msg, Client& cl) {
    MessageOut m1; m1.addTarget(cl.GetFd()); m1.setMessage(RPL_LISTSTART(cl.GetNickname()) + SEPARATOR); push_message(m1);
    if (msg.args.size() < 1) {
        for (size_t i = 0; i < Channels_.size(); i++) {
            std::stringstream ss; ss << Channels_[i].getClients().size();
            MessageOut m; m.addTarget(cl.GetFd()); m.setMessage(RPL_LIST(cl.GetNickname(), Channels_[i].getName(), ss.str(), Channels_[i].getTopic()) + SEPARATOR); push_message(m);
        }
    } else {
        for (size_t i = 0; i < msg.args[0].size(); i++) {
            std::vector<Channel>::iterator chan_it = get_channel(msg.args[0][i]);
            if (chan_it != Channels_.end()) {
                std::stringstream ss; ss << chan_it->getClients().size();
                MessageOut m; m.addTarget(cl.GetFd()); m.setMessage(RPL_LIST(cl.GetNickname(), chan_it->getName(), ss.str(), chan_it->getTopic()) + SEPARATOR); push_message(m);
            }
        }
    }
    MessageOut m2; m2.addTarget(cl.GetFd()); m2.setMessage(RPL_LISTEND(cl.GetNickname()) + SEPARATOR); push_message(m2);
}

void            Server::invite(MessageIn& msg, Client& cl) {
    if (!cl.registered)
        throw Error(cl, ERR_NOTREGISTERED(cl.GetNickname()));
    if (msg.args.size() < 2)
        throw Error(cl, ERR_NEEDMOREPARAMS(cl.GetNickname(), "INITE"));

    std::vector<Channel>::iterator  chan_it = get_channel(msg.args[1][0]);
    if (chan_it == Channels_.end())
        throw Error(cl, ERR_NOSUCHCHANNEL(cl.GetNickname(), msg.args[0][0]));
    if (!chan_it->IsClientInChannel(&cl))
        throw Error(cl, ERR_NOTONCHANNEL(cl.GetNickname(), msg.args[1][0]));
    if (!chan_it->IsAnOperator(&cl))
        throw Error(cl, ERR_CHANOPRIVSNEEDED(cl.GetNickname(), msg.args[0][0]));

    std::map<int, Client>::iterator cit = get_client_by_nick(msg.args[0][0]);
    if (cit == Clients_.end())
        throw Error(cl, ERR_NOSUCHNICK(cl.GetNickname(), msg.args[0][0]));
    if (chan_it->IsClientInChannel(&cit->second))
        throw Error(cl, ERR_USERONCHANNEL(cl.GetNickname(), chan_it->getName(), msg.args[1][0]));
    chan_it->InvitClient(&cl, &cit->second);
}

void            Server::mode(MessageIn& msg, Client& cl) {
    if (!cl.registered)
        throw Error(cl.GetFd(), ERR_NOTREGISTERED((cl.GetNickname().empty() ? "*" : cl.GetNickname())));
    if (msg.args.empty() || msg.args.size() < 1 || msg.args[0].empty() || msg.args[0][0].empty())
        throw Error(cl.GetFd(), ERR_NEEDMOREPARAMS(cl.GetNickname(), "MODE"));
    
    if (msg.args[0][0][0] != '#') return ;
    std::vector<Channel>::iterator chan_it = get_channel(msg.args[0][0]);
    if (chan_it == Channels_.end())
        throw Error(cl.GetFd(), ERR_NOSUCHCHANNEL(cl.GetNickname(), msg.args[0][0]));
    
    if (msg.args.size() == 1 || (msg.args[1].empty() || msg.args[1][0].empty())) {
        std::string response = "+"; int mode = chan_it->getMode();
        
        if (mode & MODE_INVITE_ONLY) response += 'i';
        if (mode & MODE_TOPIC) response += 't';
        if (mode & MODE_KEY) response += 'k';
        if (mode & MODE_USER_LIMIT) response += 'l';

        if (mode & MODE_KEY) response += " " + chan_it->getKey();
        if (mode & MODE_USER_LIMIT) response += " " + chan_it->getUserlimit();
        MessageOut m; m.addTarget(cl.GetFd()); m.setMessage(std::string("324 ") + cl.GetNickname() + " " + chan_it->getName() + " " + response + SEPARATOR);
        push_message(m);
    } else {
        if (!chan_it->IsAnOperator(&cl)) throw Error(cl.GetFd(), ERR_CHANOPRIVSNEEDED(cl.GetNickname(), chan_it->getName()));
        
        bool sign = true; 
        std::string smode = msg.args[1][0]; 
        size_t p = 2;

        std::string applied;
        std::string appliedParams;
        char lastSign = 0;

        for (size_t i = 0; i < smode.size(); i++) {
            char c = smode[i];
            bool changed = false;
            std::string param = "";

            switch (c)
            {
            case '+': sign = true; break;
            case '-': sign = false; break;

            case 't':
                changed = chan_it->bitMode(MODE_TOPIC, sign);
                break ;
            case 'i':
                changed = chan_it->bitMode(MODE_INVITE_ONLY, sign);
                break;
            
            case 'k':
            {
                if (sign) {
                    if (p >= msg.args.size()) { MessageOut m; m.addTarget(cl.GetFd()); m.setMessage(ERR_NEEDMOREPARAMS(cl.GetNickname(), "MODE") + SEPARATOR); push_message(m); continue; }
                    param = msg.args[p][0]; p++;
                    if (param.empty()) {continue;}
                    chan_it->SetKey(param);
                    changed = true;
                } else {
                    if (p < msg.args.size()) p++;
                    changed = chan_it->bitMode(MODE_KEY, false);
                    chan_it->RemoveKey();
                }
                break ;
            }

            case 'l':
            {
                if (sign) {
                    if (p >= msg.args.size()) { MessageOut m; m.addTarget(cl.GetFd()); m.setMessage(ERR_NEEDMOREPARAMS(cl.GetNickname(), "MODE") + SEPARATOR); push_message(m); continue; }
                    param = msg.args[p][0]; p++;
                    if (!is_unsigned_int(param)) continue ;
                    unsigned int limit = ::atoi(param.c_str());
                    if (limit == 0) continue;
                    chan_it->SetUserLimit(limit);
                    changed = true;
                } else {
                    changed = chan_it->bitMode(MODE_USER_LIMIT, false);
                    if (changed) chan_it->RemoveUserLimit();
                }
                break ;
            }

            case 'o':
            {
                if (p >= msg.args.size()) { MessageOut m; m.addTarget(cl.GetFd()); m.setMessage(ERR_NEEDMOREPARAMS(cl.GetNickname(), "MODE") + SEPARATOR); push_message(m); continue; }
                param = msg.args[p][0]; p++;
                std::map<int, Client>::iterator cit = get_client_by_nick(param);
                if (cit == Clients_.end()) {MessageOut m; m.addTarget(cl.GetFd()); m.setMessage(ERR_NOSUCHNICK(cl.GetNickname(), param) + SEPARATOR); push_message(m); continue; }
                if (chan_it->getClientByNick(param) == chan_it->getClients().end()) {MessageOut m; m.addTarget(cl.GetFd()); m.setMessage(ERR_USERNOTINCHANNEL(cl.GetNickname(), param, chan_it->getName()) + SEPARATOR); push_message(m); continue; }
                if (sign) {
                    changed = chan_it->GiveOperatorPrivilege(&cit->second);
                } else {
                    changed = chan_it->TakeOperatorPrivilege(&cit->second);
                } break;
            }
            
            default:
                MessageOut m; m.addTarget(cl.GetFd()); m.setMessage(std::string("472 " + cl.GetNickname() + ' ' + c + " :is unknow char to me" + SEPARATOR)); push_message(m);
                break;
            }
            
            if (changed) {
                char s = sign ? '+' : '-';
                if (s != lastSign) {applied += s; lastSign = s;}
                applied += c;
                if (!param.empty()) appliedParams += " " + param;
            }
        }
        if (!applied.empty()) {
            MessageOut m; m.setMessage((PREFIX(cl.GetNickname(), cl.GetUsername(), cl.GetIP())) + " MODE " + chan_it->getName() + " " + applied + appliedParams + SEPARATOR);
            for (std::vector<Client*>::iterator it = chan_it->getClients().begin(); it != chan_it->getClients().end(); it++) {
                m.addTarget((*it)->GetFd());
            }
            push_message(m);
        }
    }
}

void            Server::kick(MessageIn& msg, Client& cl) {
    if (!cl.registered)
        throw Error(cl, ERR_NOTREGISTERED(cl.GetNickname()));
    if (msg.args.size() < 2)
        throw Error(cl, ERR_NEEDMOREPARAMS(cl.GetNickname(), "KICK"));

    std::vector<Channel>::iterator  chan_it = get_channel(msg.args[0][0]);
    if (chan_it == Channels_.end())
        throw Error(cl, ERR_NOSUCHCHANNEL(cl.GetNickname(), msg.args[0][0]));
    if (!chan_it->IsAnOperator(&cl))
        throw Error(cl, ERR_CHANOPRIVSNEEDED(cl.GetNickname(), msg.args[0][0]));
    std::string comment = "";
    if (msg.args.size() > 2 && !msg.args[2].empty() && !msg.args[2][0].empty()){ 
        comment = msg.args[2][0];
    }
    for (size_t i = 0; i < msg.args[1].size(); i++) {
    try {
            std::map<int, Client>::iterator cit = get_client_by_nick(msg.args[1][i]);
            if (cit == Clients_.end())
                throw Error(cl, ERR_NOSUCHNICK(cl.GetNickname(), msg.args[1][i]));
            if (!chan_it->IsClientInChannel(&cit->second))
                throw Error(cl, ERR_USERNOTINCHANNEL(cl.GetNickname(), cit->second.GetNickname(), msg.args[0][0]));
            chan_it->KickClient(&cl, &cit->second, comment);
        }
    catch(const Error& e) 
        {
            MessageOut  m; m.addTarget(e._client.GetFd()); m.setMessage(e._msg + SEPARATOR);
            this->push_message(m);
        }
    }
    if (chan_it->getClients().size() == 0) {
        Channels_.erase(chan_it);
        return ;
    }
}

void            Server::part(MessageIn& msg, Client& cl)
{
     if (!cl.registered)
        throw Error(cl, ERR_NOTREGISTERED(cl.GetNickname()));
    if (msg.args.size() < 1)
        throw Error(cl, ERR_NEEDMOREPARAMS(cl.GetNickname(), "PART"));
    std::string reason = "";
    if (msg.args.size() >= 2 && !msg.args[1].empty() && !msg.args[1][0].empty()){ 
        reason = msg.args[1][0];}
    for (size_t i = 0; i < msg.args[0].size(); i++) {
        try {
            std::vector<Channel>::iterator  chan_it = get_channel(msg.args[0][i]);
            if (chan_it == Channels_.end())
                throw Error(cl, ERR_NOSUCHCHANNEL(cl.GetNickname(), msg.args[0][i]));
            if (!chan_it->IsClientInChannel(&cl))
                throw Error(cl, ERR_NOTONCHANNEL(cl.GetNickname(), msg.args[0][i]));
            chan_it->PartClient(&cl, reason);
            if (chan_it->getClients().size() == 0) {
                Channels_.erase(chan_it);
            }
        }
        catch(const Error& e) 
        {
            MessageOut  m; m.addTarget(e._client.GetFd()); m.setMessage(e._msg + SEPARATOR);
            this->push_message(m);
        }
    }
}
