 ///
 /// @file    TestMylogger.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2020-01-10 11:30:58
 ///
 
#include "Mylogger.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;



void test3()
{
	//仅仅只能记录字符串
	LogWarn("hello,warn,%d",100);
    LogError("hello,error,%d",200);
    //LogInfo("hello,info,%s","hello");
}

//void test4()
//{
//	int number = 1;
//	LogWarn("hello, %d", nubmer);//error
//}

void test1()
{
	cout << __FILE__ << endl;
	cout << __LINE__ << endl;
	cout << __func__ << endl;
	cout << __FUNCTION__ << endl;
}
 
int main(void)
{
	//test0();
	//test1();
	//test2();
	test3();
	return 0;
}
