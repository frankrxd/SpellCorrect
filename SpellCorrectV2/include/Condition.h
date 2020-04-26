#pragma once
#include "Noncopyable.h"

#include <pthread.h>


class MutexLock;
class Condition
:Noncopyable
{
public:
    Condition(MutexLock & mutex); 
    ~Condition();
    void wait();
    void notify();
    void notifyall();
private:
    MutexLock & _mutex;
    pthread_cond_t _cond;
};

