#include "MessageOut.hpp"
#include <algorithm>
#include <sys/socket.h>


/******CANONICAL******/
MessageOut::MessageOut( void ): message_(), targets_() {}
MessageOut::MessageOut( std::string message, std::vector<int> clients): message_(message), targets_(clients) {}
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
const std::vector<int>          MessageOut::getTargets( void ) const { return this->targets_; }
const std::string               MessageOut::getMessage( void ) const { return this->message_; }

/******SETTERS******/
void                            MessageOut::setMessage( std::string message ) { this->message_ = message; }
void                            MessageOut::addTarget( int fd ) { 
    if (std::find(targets_.begin(), targets_.end(), fd) == targets_.end()) { targets_.push_back(fd); }
}

/******METHODS******/
void                            MessageOut::send( void ) {
    for (std::vector<int>::iterator it = targets_.begin(); it != targets_.end(); it++) {
        ::send((*it), message_.c_str(), message_.size(), 0);
    }
}

