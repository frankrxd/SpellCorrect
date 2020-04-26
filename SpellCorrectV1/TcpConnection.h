#pragma once

#include "Socket.h"
#include "InetAddress.h"
#include "SocketIO.h"

#include <functional>
#include <memory>
using std::pair;
using std::function;
using std::shared_ptr;


class Eventloop;
class TcpConnection
:public std::enable_shared_from_this<TcpConnection>
{
public:
    using TcpConnectionCallback = std::function<void(const std::shared_ptr<TcpConnection>&)>;    
    explicit
    TcpConnection(int peerfd,Eventloop &); 
    ~TcpConnection() {}

    string receive();
    void send(const string & msg);
    void shutdown();
    bool isClosed();
    string connectionInfo() const;
    void sendInLoop(const string & msg);
	
    void setConnectionCallback(TcpConnectionCallback && cb)
	{	_onConnectionCallback = std::move(cb);	}

	void setMessageCallback(TcpConnectionCallback && cb)
	{	_onMessageCallback = std::move(cb);	}

	void setCloseCallback(TcpConnectionCallback && cb)
	{	_onCloseCallback = std::move(cb);	}


    string peeraddr()
    {
        return _peerAddr.ip();
    }
    unsigned short peerport()
    {
        return _peerAddr.port();
    }
    void handleConnectionCallback();
	void handleMessageCallback();
	void handleCloseCallback();
private:
    pair<InetAddress,InetAddress> pairInetAddress();
private:

    int _fd;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    SocketIO _socketIO;
    bool _isShutdownWrite;
    
    TcpConnectionCallback _onConnectionCallback;
	TcpConnectionCallback _onMessageCallback;
	TcpConnectionCallback _onCloseCallback;
    
    Eventloop & _eventloop;
};

