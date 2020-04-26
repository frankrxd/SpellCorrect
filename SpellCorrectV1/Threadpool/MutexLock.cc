#include "MutexLock.h"

#include <error.h>
#include <stdio.h>

MutexLock::MutexLock()
{
	if(pthread_mutex_init(&_mutex, NULL))
	{
		perror("pthread_mutex_init");
	}
}

void MutexLock::lock()
{
	if(pthread_mutex_lock(&_mutex))
	{
		perror("pthread_mutex_lock");
	}
}

void MutexLock::unlock()
{
	if(pthread_mutex_unlock(&_mutex))
	{
		perror("pthread_mutex_unlock");
	}
}

MutexLock::~MutexLock()
{
	if(pthread_mutex_destroy(&_mutex))
	{
		perror("pthread_mutex_destroy");
	}
}

