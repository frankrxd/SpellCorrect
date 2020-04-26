#pragma once

#include "TaskQueue.h"
#include "Thread.h"
#include <vector>
#include <memory>
#include <functional>

using std::unique_ptr;
using std::vector;

class Threadpool
{
public:
    Threadpool(size_t threadNum,size_t queSize);
    ~Threadpool() {
        if(!_isExit)
            stop();
    }
    
    void addTask(TaskCallback &&);
    void start();
    void stop();

private:
    void threadFunc();
    TaskCallback getTask();

private:
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskQueue;
    bool _isExit;
};

