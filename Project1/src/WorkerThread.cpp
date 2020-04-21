#include "WorkerThread.h"
#include "Threadpool.h"


using namespace wd;

void WorkerThread::run()
{
	_threadpool.threadFunc();
}
