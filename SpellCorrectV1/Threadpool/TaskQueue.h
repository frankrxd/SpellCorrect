#pragma once

#include "MutexLock.h"
#include "Condition.h"

#include <functional>
#include <queue>

using TaskCallback = std::function<void()>;

class TaskQueue
{
public:
    TaskQueue(size_t size) 
    :_queSize(size)
    ,_queue()
    ,_mutex()
    ,_notFull(_mutex)
    ,_notEmpty(_mutex)
    ,_isUsed(true){}
    
    ~TaskQueue() {}
    
    bool full() const;
    bool empty() const;
    void push(const TaskCallback &);
    TaskCallback pop();

    void wakeup();//激活所有等待_notEmpty条件变量的线程
private:
    size_t _queSize;
    std::queue<TaskCallback> _queue;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    bool _isUsed;
};

