#include "Threadpool.h"

#include <unistd.h>

Threadpool::Threadpool(size_t threadNum,size_t queSize)
    :_threadNum(threadNum)
     ,_queSize(queSize)
     ,_threads()
     ,_taskQueue(_queSize)
     ,_isExit(false)
{
    _threads.reserve(threadNum);
}

void Threadpool::start()
{
    for(size_t idx = 0; idx != _threadNum; ++idx) {
		unique_ptr<Thread> threadfunc(new Thread(
			std::bind(&Threadpool::threadFunc, this)
		));
		_threads.push_back(std::move(threadfunc));
	}

	for(auto & threadPtr : _threads)
		threadPtr->start();
}

void Threadpool::addTask(TaskCallback && task)
{
    _taskQueue.push(std::move(task));
}

TaskCallback Threadpool::getTask()
{
    return _taskQueue.pop();
}

void Threadpool::threadFunc()
{
    while(!_isExit)
    {
        TaskCallback task = getTask();
        if(task)
            task();
    }
}

void Threadpool::stop()
{
    if(!_isExit)
    {
        while(!_taskQueue.empty())
            ::usleep(100);
        _isExit = true;
        _taskQueue.wakeup();

        for(auto & threadptr:_threads)
            threadptr->join();
    }

}

