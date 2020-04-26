#pragma once
#include "Socket.h"
#include "InetAddress.h"

class Acceptor
{
public:
    Acceptor(const string & ip,unsigned short port);
    Acceptor(unsigned short port);
    void ready();
    int accept();
    int fd() const {    return _sock.socketfd();  }
    void setNonblock(int );
private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();
private:
    Socket _sock;
    InetAddress _inetAddress;
};

