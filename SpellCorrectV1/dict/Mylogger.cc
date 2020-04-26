#include "../templateMylogger/Mylogger.h"
#include <iostream>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>

using std::cout;
using std::endl;

Mylogger * Mylogger::_pInstance = nullptr;

Mylogger::Mylogger()
: _mycat(Category::getRoot().getInstance("project1"))
{
	PatternLayout * ptnLayout1 = new PatternLayout();
	ptnLayout1->setConversionPattern("%d{%Y-%m-%d %H:%M:%S} %c [%p] %m%n");

	PatternLayout * ptnLayout2 = new PatternLayout();
	ptnLayout2->setConversionPattern("%d %c [%p] %m%n");

	OstreamAppender * osAppender = new OstreamAppender("os", &cout);
	osAppender->setLayout(ptnLayout1);

	FileAppender * fileAppender = new FileAppender("file", "project1.log");
	fileAppender->setLayout(ptnLayout2);
	
	_mycat.addAppender(osAppender);
	_mycat.addAppender(fileAppender);
	_mycat.setPriority(Priority::DEBUG);
	cout << "Mylogger()" << endl;
}

Mylogger::~Mylogger()
{
	Category::shutdown();	
	cout << "~Mylogger()" << endl;
}
