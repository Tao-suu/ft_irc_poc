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
    addr_.sin_addr.s_addr = INADDR_ANY;
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

        for (int i = 0; i < pollfds_.size(); i++)
        {
            if (pollfds_[i].revents == 0) continue;     //  le client/serveur n'a fait aucun action ou recu aucune acction
            
            if (pollfds_[i].fd == servFd_ && pollfds_[i].revents == POLLIN) {}     // Le serveur recoit un nouveau client

            else {
                if (pollfds_[i].revents & (POLLIN | POLLERR)) {}   // Supprimer le client
                else if (pollfds_[i].revents & POLLIN) {}   // Recuperer les donnees
            }
        }
    }
}


int                 Server::get_port( void ) const {return port_;}
const std::string&  Server::get_password( void ) const {return pass_;}


std::ostream&   operator<<(std::ostream& os, const Server& s)
{
    os << "Serveur: port=" << s.get_port() << " | pass=" << s.get_password();
    return os;
}



Server::ServerException::ServerException( const std::string& message ): message_(message) {}
const char* Server::ServerException::what() const throw() {return message_.c_str();}
Server::ServerException::~ServerException() throw() {}