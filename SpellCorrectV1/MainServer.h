#pragma once
#include "TcpServer.h"
#include "TcpConnection.h"
#include "Eventloop.h"
#include "./hiredis/hiredis.h"
#include "Work.h"
#include "./Threadpool/Threadpool.h"
#include "./templateMylogger/Mylogger.h"
#include <unistd.h>
#include <iostream>
#include <memory>
#include <queue>
#include <iomanip>
using std::unique_ptr;
using std::priority_queue;

struct singleResult;
class MyTask
{
public:
	MyTask(const string & request, shared_ptr<TcpConnection> connection,MyWork * mywork,redisContext * redisconn)
	: _request(request)
	, _connection(connection)
    , _mywork(mywork)
    , _redisconn(redisconn)
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
        string result;
        result.clear();
        string redisRequest = "get "+_request;
        redisReply * reply = (redisReply *)redisCommand(_redisconn,redisRequest.c_str());
        redisRequest.clear();

        if(reply->type == 1)
        {
            LogInfo("redis server hit cache!  [request:%s] [response:%s]",_request.c_str(),reply->str);
            result = reply->str;
            freeReplyObject(reply);
            return result;
        }
        freeReplyObject(reply);
        
        vector<singleResult> workResult = _mywork->handle_request(_request);
        size_t maxstringlength = 0;
        for(auto temp:workResult)
        {
            if(temp._word.size()>maxstringlength)
            maxstringlength = temp._word.size();
        }
        vector<singleResult>::reverse_iterator iter;//方向迭代器
        for(iter=workResult.rbegin();iter!=workResult.rend();++iter)
        {
            result.append("(").append(iter->_word).append(")");
            std::cout<<"word:"<<std::setw(maxstringlength+2)<<std::left<<iter->_word<<" | "
                <<"freq:"<<std::setw(5)<<std::left<<iter->_freq<<" | "
                <<"editDistance:"<<std::setw(2)<<std::right<<iter->_editDistance<<std::endl;
        }
        string setRedis;
        setRedis.append("set ").append(_request).append(" ").append(result);
        redisCommand(_redisconn,setRedis.c_str());
        LogInfo("redis server cache update! [response:%s]",result.c_str());
        return result;
    }
private:
	string _request;
	shared_ptr<TcpConnection> _connection;
    MyWork * _mywork;
    redisContext * _redisconn;
};

class MainServer
{
public:
    MainServer(const string &ip,unsigned short port);
    ~MainServer() {}

public:

    void start();

private:
    void Redisconnect()
    {
        _redisconn = redisConnect("127.0.0.1",6379);
        if(_redisconn != NULL && _redisconn->err)
        {
            LogError("redis connection error: %s",_redisconn->errstr);
//            cout<<"connection error: "<<_conn->errstr;
            return ;
        }
        else{
            LogInfo("redis server has connected! [127.0.0.1:6380]");
//            cout<<"redis server has connected!"<<endl;
        }
    }

private:

    string _ip;
    unsigned short _port;
    TcpServer _server;
    MyWork _mywork;
    Threadpool * _threadpoolPtr;
    redisContext * _redisconn;
};

