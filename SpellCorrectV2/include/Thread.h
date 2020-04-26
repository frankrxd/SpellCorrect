#pragma once

#include "Noncopyable.h"

#include <pthread.h>
#include <functional>

using std::function;

class Thread
:Noncopyable
{
public:
    using ThreadCallback = std::function<void()>;
	Thread(ThreadCallback && callback);
    ~Thread();

    void start();//开启子线程
    void join();//回收子线程
    bool _isRUnning() const {     return _isRunning;  }
    

private:
    static void * threadFunc(void *);
private:
    ThreadCallback _callback;
    pthread_t _pthid;
    bool _isRunning;
    
};

