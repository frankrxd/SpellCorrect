#include "Acceptor.h"

Acceptor::Acceptor(const string & ip,unsigned short port)
    :_sock()
     ,_inetAddress(ip,port){}

Acceptor::Acceptor(unsigned short port)
    :_sock()
     ,_inetAddress(port){}

void Acceptor::setReuseAddr()
{
    int reuse = 1;
    if(setsockopt(fd(),SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int))){
        ERROR_LOG("setReuseAddr");
        ::close(_sock.socketfd());
    }
}

void Acceptor::setReusePort()
{
    int reuse = 1;
    if(setsockopt(fd(),SOL_SOCKET,SO_REUSEPORT,&reuse,sizeof(int))){
        ERROR_LOG("setReusePort");
        ::close(_sock.socketfd());
    }
}

void Acceptor::bind()
{
    int ret = ::bind(fd(),(const struct sockaddr *)_inetAddress.getSockaddrPtr(),sizeof(InetAddress));
    if(ret == -1){
        ERROR_LOG("bind");
        ::close(_sock.socketfd());
        exit(EXIT_FAILURE);
    }
}

void Acceptor::listen()
{
    int ret = ::listen(fd(),10);
    if(ret == -1){
        ERROR_LOG("listen");
        ::close(_sock.socketfd());
        exit(EXIT_FAILURE);
    }
}

int Acceptor::accept()
{
    int peerfd = ::accept(fd(),nullptr,nullptr);
    if(peerfd == -1){
        ERROR_LOG("accept");
    }
    return peerfd;
}

void Acceptor::ready()
{
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}


void Acceptor::setNonblock(int peerfd)
{
    int flag = fcntl(peerfd,F_GETFL,0);
    if(flag == -1){
		ERROR_LOG("setNonblock");
	}
	int ret = ::fcntl(peerfd, F_SETFL, O_NONBLOCK | flag);
	if(ret == -1) {
		ERROR_LOG("setNonblock");
	}
}
