#include "MainServer.h"
#include <functional>
using std::function;

Threadpool * g_threadpool = nullptr;
redisContext * g_redisconn = nullptr;
MyWork *g_work = nullptr;
MainServer::MainServer(const string &ip
                       ,unsigned short port)
    :_ip(ip)
    ,_port(port)
    ,_server(ip,port)
    ,_mywork()
    ,_threadpoolPtr(new Threadpool(4,20))
{   
    Redisconnect();
}


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
     
    MyTask task(msg,connection,g_work,g_redisconn);
    g_threadpool->addTask(std::bind(&MyTask::process,task));
}

void onClose(const shared_ptr<TcpConnection> & connection)
{
    LogInfo("%s has closed!",connection->connectionInfo().c_str());
}


void MainServer::start()
{
    LogInfo("server started");
    _mywork.init();
    _threadpoolPtr->start();
    g_work = &_mywork;
    g_threadpool = _threadpoolPtr;
    g_redisconn = _redisconn;
    _server.setConnectionCallback(&onConnection);
    _server.setMessageCallback(&onMessage);
    _server.setCloseCallback(&onClose);
    _server.start();

}
