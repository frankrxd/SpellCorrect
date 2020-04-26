#include "Eventloop.h"

#include <sys/eventfd.h>
#include <memory>

using std::cout;
using std::endl;
using std::shared_ptr;
Eventloop::Eventloop(Acceptor & acceptor)
:_acceptor(acceptor)
,_epollfd(createEpollfd())
,_eventfd(createEventfd())
,_events(1024)
,_islooping(false){
    addEpollReadfd(_acceptor.fd());
    addEpollReadfd(_eventfd);
}

void Eventloop::loop()
{
    _islooping = true;
    while(_islooping)
    {
        waitEpoll();
    }
}

void Eventloop::stop()
{
    if(_islooping)
        _islooping = false;
}

void Eventloop::runInLoop(Functor && cb)
{
	//成为临界资源
	{
		MutexLockGuard autolock(_mutex);
		_pendingFunctors.push_back(std::move(cb));
	}
	wakeup();//通知IO线程
}

int Eventloop::createEpollfd()
{
    int fd = epoll_create1(0);                                       
    if(fd == -1)
        ERROR_LOG("epoll_create1");
    return fd;
}

int Eventloop::createEventfd()
{
    int fd = ::eventfd(0, 0);
	if(fd == -1) {
        ERROR_LOG("createEventfd")
	}
	return fd;
}

void Eventloop::addEpollReadfd(int fd)
{
    struct epoll_event temp;
    temp.events = EPOLLIN;
    temp.data.fd = fd;
    if(epoll_ctl(_epollfd,EPOLL_CTL_ADD,temp.data.fd,&temp) == -1 )
        ERROR_LOG("epoll_ctl: listenfd");
}

void Eventloop::delEpollReadfd(int fd)
{
    if(epoll_ctl(_epollfd,EPOLL_CTL_DEL,fd,nullptr) == -1 )
        ERROR_LOG("epoll_ctl: listenfd");
}

void Eventloop::waitEpoll()
{
    int ready;
    do{
        ready = epoll_wait(_epollfd,&*_events.begin(),_events.size(),5000);
    }while(ready == -1 && errno == EINTR);
    LogDebug("Epoll retval = %d",ready);
    if(ready == -1) {  
        ERROR_LOG("epoll");
    }
    else if(ready == 0)
        LogDebug("Epoll timeout!")
    else {//ready>0
        for(size_t i=0;i<ready;++i)
        {
            if(_events[i].data.fd == _acceptor.fd()&& 
               (_events[i]).events & EPOLLIN)
            {
                //处理新连接
                handleNewConnection();
            }
            else if(_events[i].data.fd == _eventfd)
            {
                handleRead();
                doPeningFunctors();
            }
            else
            {
                //处理已建好的连接
                handleMessage(_events[i].data.fd);
            }
        }//end of for
    }//end of if

}

void Eventloop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    _acceptor.setNonblock(peerfd);
    addEpollReadfd(peerfd);
    shared_ptr<TcpConnection> connection(new TcpConnection(peerfd,*this));
    _connections[peerfd] = connection;
    
    connection->setConnectionCallback(_onConnectionCallback);
	connection->setMessageCallback(_onMessageCallback);
	connection->setCloseCallback(_onCloseCallback);
    connection->handleConnectionCallback();
}

void Eventloop::handleMessage(int peerfd)
{
    auto iter = _connections.find(peerfd);
    if(iter != _connections.end())
    {
        auto x = iter->second;
        if(!x->isClosed())
        {
            x->handleMessageCallback();
        }
        else
        {
            x->handleCloseCallback();
            delEpollReadfd(peerfd);
            _connections.erase(peerfd);
        }
    }
}

void Eventloop::wakeup()
{
	uint64_t one = 1;
	int ret = ::write(_eventfd, &one, sizeof(one));
	if(ret != sizeof(one)) {
		ERROR_LOG("write eventfd");
	}
}

void Eventloop::handleRead()
{
    uint64_t temp = 0;
	int ret = ::read(_eventfd, &temp, sizeof(temp));
	if(ret != sizeof(temp)) {
		ERROR_LOG("read eventfd");
	}
}
void Eventloop::doPeningFunctors()
{
	vector<Functor> tmp;
	{
		MutexLockGuard autolock(_mutex);
		tmp.swap(_pendingFunctors);
	}

	for(auto & functor : tmp)
		functor();
}
