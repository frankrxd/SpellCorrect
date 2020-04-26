#pragma once
#include "Acceptor.h"
#include "Eventloop.h"
#include <functional>

using std::function;

class TcpServer
{
public:
    using TcpConnectionCallback = function<void(const shared_ptr<TcpConnection>)>;
    TcpServer(const string & ip,unsigned short port) 
    :_acceptor(ip,port)
    ,_eventloop(_acceptor){}

    ~TcpServer() {}
    
    void start()
    {
        _acceptor.ready();
        _eventloop.loop();
    }

    void setConnectionCallback(TcpConnectionCallback && cb)
    {	_eventloop.setConnectionCallback(std::move(cb));	}

	void setCloseCallback(TcpConnectionCallback && cb)
	{	_eventloop.setCloseCallback(std::move(cb));	}

	void setMessageCallback(TcpConnectionCallback && cb)
	{	_eventloop.setMessageCallback(std::move(cb));	}

private:
    Acceptor _acceptor;
    Eventloop _eventloop;
};

