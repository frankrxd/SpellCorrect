#include "TcpServer.h"
#include "TcpConnection.h"
#include "Eventloop.h"

#include "./Threadpool/Threadpool.h"
#include "./templateMylogger/Mylogger.h"
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;

class MyTask
{
public:
	MyTask(const string & request, shared_ptr<TcpConnection> connection)
	: _request(request)
	, _connection(connection)
	{}

	//process方法是线程池中的子线程来执行的
	void process()
	{
		//.....do something;
		//decode compute  encode
		string response = doTask();//业务逻辑处理
		
		//_conn->send(response);//不能直接发送，因为现在处于计算线程，并不在IO线程
		//这里涉及到线程间通信: 计算线程要通知IO线程发送数据
		_connection->sendInLoop(response);
	}

    string doTask(){
        return _request;
        //return string("hello");
    }
private:
	string _request;
	shared_ptr<TcpConnection> _connection;
};

unique_ptr <Threadpool> threadpoolPtr(new Threadpool(4,10));

void onConnection(const shared_ptr<TcpConnection> & connection)
{
    LogInfo("%s has connected!",connection->connectionInfo().c_str());
}

void onMessage(const shared_ptr<TcpConnection> & connection)
{
    string recvmsg = connection->receive();
    string msg = recvmsg.substr(0,recvmsg.size()-1);
    LogInfo("server receive: %s [from %s:%u]"
            ,msg.c_str(),
            connection->peeraddr().c_str(),
            connection->peerport());

    MyTask task(msg,connection);
    threadpoolPtr->addTask(std::bind(&MyTask::process,task));
}

void onClose(const shared_ptr<TcpConnection> & connection)
{
    LogInfo("%s has closed!",connection->connectionInfo().c_str());
}
void test0(void)
{
    TcpServer server("192.168.0.103",8888);
    threadpoolPtr->start();
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.setCloseCallback(onClose);

    server.start();
}
 
int main(void)
{
	test0();
	return 0;
}
