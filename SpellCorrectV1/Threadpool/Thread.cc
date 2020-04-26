#include "Thread.h"

Thread::Thread(ThreadCallback && callback)
: _callback(std::move(callback))
, _pthid(0)
, _isRunning(false)
{
	
}
void Thread::start()
{
    pthread_create(&_pthid,NULL,threadFunc,this);
    _isRunning = true;
}

void * Thread::threadFunc(void * arg)
{
    Thread * p = static_cast<Thread *>(arg);
    if(p)
        p->_callback();//调用回调函数
    return nullptr;
}

void Thread::join()
{
    if(_isRunning)
    {
        pthread_join(_pthid,nullptr);
        _isRunning = false;
    }
}

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_pthid);
        _isRunning = false;
    }
}

