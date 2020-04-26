#pragma once
#include "Mylogger.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

class Socket
{
public:
    Socket();
    void shutdownWrite();
    ~Socket() {
        ::close(_sockedfd);
    }
    int socketfd() const;

private:
    int _sockedfd;
};

