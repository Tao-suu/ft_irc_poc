#include "Server.hpp"

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

    while (1)
    {
        // errno = 0;
        int ret = poll(&pollfds_[0], pollfds_.size(), -1);
        if (ret == -1)
        {
            if (errno == EINTR) continue;
            else break;
        }

        for (unsigned int i = 0; i < pollfds_.size(); i++)
        {
            if (pollfds_[i].revents == 0) continue;     //  le client/serveur n'a fait aucun action ou recu aucune acction
            
            if (pollfds_[i].fd == servFd_ && pollfds_[i].revents == POLLIN) { acceptNewClient(); }     // Le serveur recoit un nouveau client

            else {
                if (pollfds_[i].revents & (POLLHUP | POLLERR)) { toRemove_.push_back(pollfds_[i].fd); }   // Supprimer le client
                else if (pollfds_[i].revents & POLLIN) { handleClientData(pollfds_[i].fd); }   // Recuperer les donnees
            }
        }
        for (unsigned int i = 0; i < toRemove_.size(); i++)
        {
            int fd = toRemove_[i];
            Clients_[fd]._in_buffer.erase();
            Clients_.erase(fd);
            close(fd);
            std::cout << "client disconnected\tfd = " << fd << std::endl;
            for (unsigned int j = 0; j < pollfds_.size(); j++)
            {
                if (pollfds_[j].fd == fd) {
                    pollfds_.erase(pollfds_.begin() + j); 
                    break;
                }
            }
        }
        toRemove_ = std::vector<int>();
        send_all();          
    }
}


int                 Server::get_port( void ) const {return port_;}
const std::string&  Server::get_password( void ) const {return pass_;}
const std::string   Server::get_ip( void ) const { return std::string(inet_ntoa(addr_.sin_addr)); }


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

    pollfd new_fd;
    new_fd.fd = fd;
    new_fd.revents = 0;
    new_fd.events = POLLIN;
    pollfds_.push_back(new_fd);

    Clients_[fd] = Client(fd);
	if (pass_.empty())
		Clients_[fd].pass_ok = true;
    std::cout << "new client fd = " << fd << "\taddr_ = " << inet_ntoa(addr.sin_addr) << std::endl;
}

void                Server::handleClientData( int fd )
{
    char    buffer[2048];
    std::memset(buffer, 0, 2048);

    ssize_t bytes = recv(fd, buffer, 2048, 0);

    std::string buff(buffer);
    std::cout << "client fd(" << fd << ") buffer_in += " << buff << std::endl;

    if (bytes == 0) {toRemove_.push_back(fd);}
    if (bytes > 0)  Clients_[fd]._in_buffer += buffer; 
    if (bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK) return ;
        std::cout << "recv failed" << std::endl;
        return ;
    }

    size_t  pos;
    while ((pos = Clients_[fd]._in_buffer.find(SEPARATOR)) != std::string::npos)
    {
        std::string line = Clients_[fd]._in_buffer.substr(0, pos);
        Clients_[fd]._in_buffer = Clients_[fd]._in_buffer.substr(pos + SEPARATOR.size(), Clients_[fd]._in_buffer.size() - (pos + SEPARATOR.size()));
        std::cout << "client fd(" << fd << ") : " << line << std::endl;
		if (!cv.validateContent(line))
			return ;
		MessageIn	msg = cv.parseContent(line);
		exec(msg, Clients_[fd]);
    }
}
void				Server::exec(MessageIn &msg, Client& sender)
{
	try {
		// if (msg.cmdName != "PASS" && msg.cmdName != "NICK" && msg.cmdName != "USER" && !sender.pass_ok)
		// 	throw Error(sender, ERR_NOTREGISTERED((sender.GetNickname().empty() ? "*" : sender.GetNickname())));
		if (msg.cmdName == "PASS") pass(msg, sender);
		else if (msg.cmdName == "NICK") nick(msg, sender);
		else if (msg.cmdName == "USER") user(msg, sender);
        // else if (msg.cmdName == "PING") ping(msg, sender);
		else std::cerr << "PUTE" << std::endl;
	} catch (Error &e) {
        e._msg += "\r\n";
        MessageOut  err(e._msg); err.addTarget(&e._client);
		message_stack.push(err);
    }
}

void				Server::send_error(Error &e)
{
	e._msg += "\r\n";
	send(e._client.GetFd(), e._msg.c_str(), e._msg.size(), 0);
}
void				Server::sendWelcome(Client &cl)
{
    std::cout << "sendWelcome" << std::endl;
	cl.registered = true;
    std::string     message = RPL_WELCOME(cl.GetNickname()) + "\r\n" +
                            RPL_YOURHOST(cl.GetNickname(), "ft_irc", "version") + "\r\n" +
                            RPL_CREATED(cl.GetNickname(), "datetime") + "\r\n" + 
                            RPL_MYINFO(cl.GetNickname(), "ft_irc", "version", "modes") + "\r\n";
    std::vector<Client*>    targets; targets.push_back(&cl);
    message_stack.push(MessageOut(message, targets));                     
}

Server::ServerException::ServerException( const std::string& MessageIn ): MessageIn_(MessageIn) {}
const char* Server::ServerException::what() const throw() {return MessageIn_.c_str();}
Server::ServerException::~ServerException() throw() {}


/*******************************/
/* CHANNEL AND CLIENT FUNCTION */
/*******************************/

bool 				Server::is_nickname_exist(std::string nick) {
	for (std::map<int, Client>::iterator i = Clients_.begin(); i != Clients_.end(); i++)
		if ((*i).second.GetNickname() == nick) return true;
	return false; 
}
bool                Server::is_channel_exist(std::string name) {
    for (std::vector<Channel>::iterator it = Channels_.begin(); it != Channels_.end(); it++) {
        if ((*it).getName() == name) return true;
    }
    return false;
}
std::vector<Channel>::iterator Server::get_channel(std::string name) {
    for (std::vector<Channel>::iterator it = Channels_.begin(); it != Channels_.end(); it++) {
        if ((*it).getName() == name) return it;
    }
    return Channels_.end();
}

void                Server::send_all( void ) {
    while (!message_stack.empty()) {
        MessageOut& message = message_stack.top();
        message.send();
        message_stack.pop();
    }
}





/****************************/
/*        IRC COMMAND       */
/****************************/

void				Server::pass(MessageIn &msg, Client& cl) {
    /* ERR MANAGE */
    if (msg.args.size() < 1 || msg.args[0].size() < 1)
		throw Error(cl, ERR_NEEDMOREPARAMS((cl.GetNickname().empty() ? "*" : cl.GetNickname()), "PASS"));
	if (cl.registered)
		throw Error(cl, ERR_ALREADYREGISTERED((cl.GetNickname().empty() ? "*" : cl.GetNickname())));
	if (msg.args[0][0] != this->pass_)
		throw Error(cl, ERR_PASSWDMISMATCH((cl.GetNickname().empty() ? "*" : cl.GetNickname())));

    /* COMMAND CORE */
    cl.pass_ok = true;
    cl.cap_process++;
    if (cl.cap_process >= 3) sendWelcome(cl); // Login ended
}

void				Server::nick(MessageIn &msg, Client& cl) {
	/* ERR MANAGE */
	if (msg.args.size() < 1 || msg.args[0].size() < 1)
		throw Error(cl, ERR_NONICKNAMEGIVEN((cl.GetNickname().empty() ? "*" : cl.GetNickname())));
	else if (msg.args[0][0].find(':') != std::string::npos || msg.args[0][0].find(' ') != std::string::npos)
		throw Error(cl, ERR_ERRONEUSNICKNAME((cl.GetNickname().empty() ? "*" : cl.GetNickname()), msg.args[0][0]));
	else if (is_nickname_exist(msg.args[0][0]) && msg.args[0][0] != cl.GetNickname())
		throw Error(cl, ERR_NICKNAMEINUSE((cl.GetNickname().empty() ? "*" : cl.GetNickname()), msg.args[0][0]));
	
    /* COMMAND CORE */
    cl.SetNickname(msg.args[0][0]); // All case
	if (cl.registered) {    // Change Nickname case
		/*Broadcast new name to his channel and himself*/
	} else { cl.cap_process++; }
	if (cl.cap_process >= 3 && cl.pass_ok) // Login ended
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
    cl.cap_process++;
	if (cl.cap_process >= 3) sendWelcome(cl); // Login endeded
}

/* WIP IPW PWI PIW IWP*/
/*
void                Server::join(MessageIn& msg, Client& cl) {
    if (msg.args.size() < 2)
            throw Error(cl, ERR_NEEDMOREPARAMS((cl.GetNickname().empty() ? "*" : cl.GetNickname()), "JOIN"));
    if (!cl.registered)
            throw Error(cl, ERR_NOTREGISTERED((cl.GetNickname().empty() ? "*" : cl.GetNickname())));
}

void                Server::ping(MessageIn& msg, Client& cl) {
    if (msg.args.size() < 2)
            throw Error(cl, ERR_NEEDMOREPARAMS((cl.GetNickname().empty() ? "*" : cl.GetNickname()), "PING"));
}
*/