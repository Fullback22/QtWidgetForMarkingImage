#pragma once
#include <ctime>
#include <fstream>
#include <string>

extern unsigned int levelLog;

enum class LogLevel
{
	_DEBUG_ = 1,
	_ERROR_ = 2,
	_INFO_ = 3,
	_FATAL_ = 4
};

class SimplLoger
{
	std::string logFileName;
	std::ofstream logOut;
public:
	SimplLoger();
	~SimplLoger();
	void logMessege(const std::string& messeg, const LogLevel level);//logType 1-
	void logInit(const LogLevel targetLevel);
};

extern SimplLoger LOG;