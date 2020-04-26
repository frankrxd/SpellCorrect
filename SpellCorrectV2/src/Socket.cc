#include "Socket.h"
Socket::Socket() {
    _sockedfd = socket(AF_INET,SOCK_STREAM,0);
    if(_sockedfd == -1)
    {
        ERROR_LOG("socket");
        ::close(_sockedfd);
        ::exit(EXIT_FAILURE);
    }
}

int Socket::socketfd() const
{
    return _sockedfd;
}

void Socket::shutdownWrite()
{
    if(::shutdown(_sockedfd,SHUT_WR))
    {
        ERROR_LOG("shutdownWrite");
    }
}
