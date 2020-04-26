#pragma once
#include <sys/epoll.h>
#include "Acceptor.h"
#include "TcpConnection.h"
#include "MutexLock.h"
#include <vector>
#include <unordered_map>
#include <memory>
using std::unordered_map;
using std::vector;
using std::shared_ptr;
class Eventloop
{
public:
    using TcpConnectionCallback = function<void(const shared_ptr<TcpConnection>)>;
    using Functor = function<void()>;
    Eventloop(Acceptor &);
    ~Eventloop() {
        ::close(_epollfd);
    }
    void loop();
    void stop();
    
    void runInLoop(Functor && cb);
	void doPeningFunctors();

    void setConnectionCallback(TcpConnectionCallback && cb)
	{	_onConnectionCallback = std::move(cb);	}

	void setMessageCallback(TcpConnectionCallback && cb)
	{	_onMessageCallback = std::move(cb);	}

	void setCloseCallback(TcpConnectionCallback && cb)
	{	_onCloseCallback = std::move(cb);	}

private:
    int createEpollfd();
    int createEventfd();
    void addEpollReadfd(int fd);
    void delEpollReadfd(int fd);
    void waitEpoll();
    void handleNewConnection();
    void handleMessage(int );
     
    void wakeup();
    void handleRead();
private:
    Acceptor & _acceptor;
    int _epollfd;
    int _eventfd;
    unordered_map<int,shared_ptr<TcpConnection>> _connections;
    vector<struct epoll_event> _events;
    bool _islooping;
    
    MutexLock _mutex;
    vector<Functor> _pendingFunctors;

    TcpConnectionCallback _onConnectionCallback;
	TcpConnectionCallback _onMessageCallback;
	TcpConnectionCallback _onCloseCallback;
};

