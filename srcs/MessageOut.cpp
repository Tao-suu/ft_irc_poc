#include "MessageOut.hpp"
#include <algorithm>
#include <sys/socket.h>


/******CANONICAL******/
MessageOut::MessageOut( void ): message_(), targets_() {}
MessageOut::MessageOut( std::string message, std::vector<Client*> clients): message_(message), targets_(clients) {}
MessageOut::MessageOut( const MessageOut& o ): message_(o.message_), targets_(o.targets_) {}
MessageOut::~MessageOut( void ) {}

MessageOut&                     MessageOut::operator=( const MessageOut& o ) {
    if (this != &o ) {
        message_ = o.message_;
        targets_ = o.targets_;
    }
    return *this;
}

/******GETTERS******/
const std::vector<Client*>      MessageOut::getTargets( void ) const { return this->targets_; }
const std::string               MessageOut::getMessage( void ) const { return this->message_; }

/******SETTERS******/
void                            MessageOut::setMessage( std::string message ) { this->message_ = message; }
void                            MessageOut::addTarget( Client* cl ) { 
    if (std::find(targets_.begin(), targets_.end(), cl) == targets_.end()) { targets_.push_back(cl); }
}

/******METHODS******/
void                            MessageOut::send( void ) {
    for (std::vector<Client*>::iterator it = targets_.begin(); it != targets_.end(); it++) {
        ::send((*it)->GetFd(), message_.c_str(), message_.size(), 0);
    }
}

