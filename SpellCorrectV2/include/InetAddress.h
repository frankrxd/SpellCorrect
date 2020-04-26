#pragma once
#include <netinet/in.h>
#include <string>
using std::string;
class InetAddress
{
public:
    InetAddress(const string & ip,unsigned short port);
    InetAddress(const struct sockaddr_in & addr)
        :_addr(addr){}
    InetAddress(unsigned short port);
    string ip() const;
    unsigned short port() const;
    struct sockaddr_in * getSockaddrPtr(){  return &_addr;  }
private:
    struct sockaddr_in _addr;
};

