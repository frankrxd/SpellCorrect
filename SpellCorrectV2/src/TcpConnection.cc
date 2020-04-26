#include "TcpConnection.h"
#include "Eventloop.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

TcpConnection::TcpConnection(int peerfd,Eventloop & loop)
:_fd(peerfd)
,_localAddr(pairInetAddress().first)
,_peerAddr(pairInetAddress().second)
,_socketIO(peerfd)
,_isShutdownWrite(false)
,_eventloop(loop){
    //setNonbnlock();
}


pair<InetAddress,InetAddress> TcpConnection::pairInetAddress()
{
    struct sockaddr_in localaddr,peeraddr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ret1 = getsockname(_fd,(struct sockaddr*)&localaddr,&len);
    if(ret1 == -1){
        ERROR_LOG("getsockname");
    }
    int ret2 = getpeername(_fd,(struct sockaddr*)&peeraddr,&len);
    if(ret2 == -1){
        ERROR_LOG("getpeername");
    }
    return std::make_pair(InetAddress(localaddr),InetAddress(peeraddr));
}

string TcpConnection::connectionInfo() const
{
	std::ostringstream oss;
	oss << _localAddr.ip() << ":" << _localAddr.port() << " --> " 
		<< _peerAddr.ip() << ":" << _peerAddr.port();
	return oss.str();
}

void TcpConnection::shutdown()
{
    ::close(_fd);
}

bool TcpConnection::isClosed()
{
    return _socketIO.isClosed();
}


string TcpConnection::receive()
{
    char buff[65536] = {0};
    memset(buff,0,sizeof(buff));
    int ret = _socketIO.readline(buff,65536);
    if(ret < 0)
        close(_fd);
    else
        return string(buff);
}

void TcpConnection::send(const string & msg)
{
    int ret = _socketIO.writen(msg.data(),msg.size());
    if(ret != msg.size()){
        LogError("send msg error.");        
    }
}

void TcpConnection::handleConnectionCallback()
{
	if(_onConnectionCallback)
		_onConnectionCallback(shared_from_this());
}

void TcpConnection::handleMessageCallback()
{
	if(_onMessageCallback)
		_onMessageCallback(shared_from_this());
}
void TcpConnection::handleCloseCallback()
{
	if(_onCloseCallback)
		_onCloseCallback(shared_from_this());
}

void TcpConnection::sendInLoop(const string & msg)
{
	_eventloop.runInLoop(std::bind(&TcpConnection::send, this, msg));
}
