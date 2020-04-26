#ifndef __MYLOGGER_H__
#define __MYLOGGER_H__

#include <log4cpp/Category.hh>
#include <string.h>
using namespace log4cpp;

class Mylogger
{
public:
	static Mylogger * getInstance()
	{
		if(nullptr == _pInstance)
			_pInstance = new Mylogger();

		return _pInstance;
	}

	static void destroy()
	{
		if(_pInstance) {
			delete _pInstance;
			_pInstance = nullptr;
		}
	}
    
    template<class... Args1>
	void warn(Args1... args1)
    {
        _mycat.warn(args1...);
    }
    template<class... Args2>
	void error(Args2... args2)
    {
        _mycat.error(args2...);
    }
    
    template<class... Args3>
	void info(Args3... args3)
    {
        _mycat.info(args3...);
    }
    template<class... Args4>
	void debug(Args4... args4)
    {
        _mycat.debug(args4...);
    }

private:
	Mylogger();
	~Mylogger();

private:
	static Mylogger * _pInstance;
	//对象的数据成员
	Category & _mycat;
};

#define prefix(msg) string("[").append(__FILE__)\
			.append("|").append(__func__)\
			.append("|").append(std::to_string(__LINE__))\
			.append("] ").append(msg).c_str()


#define LogWarn(...) Mylogger::getInstance()->warn(__VA_ARGS__);
#define LogError(...) Mylogger::getInstance()->error(__VA_ARGS__);
#define LogInfo(...) Mylogger::getInstance()->info(__VA_ARGS__);
#define LogDebug(...) Mylogger::getInstance()->debug(__VA_ARGS__);

#define ERROR_LOG(msg) LogError("%s: %s\n",msg,strerror(errno)); 
#define INFO_LOG(msg) LogInfo("%s: %s\n",msg,strerror(errno)); 

//#define LogWarn(...) Mylogger::getInstance()->warn(__VA_ARGS__);
//#define LogError(msg,...) Mylogger::getInstance()->warn(prefix(msg),__VA_ARGS__);
//#define LogInfo(...) Mylogger::getInstance()->warn(prefix(msg),__VA_ARGS__);
//#define LogDebug(...) Mylogger::getInstance()->warn(prefix(msg),__VA_ARGS__);
#endif
