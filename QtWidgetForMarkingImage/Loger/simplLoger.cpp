#include "SimplLoger.h"

unsigned int levelLog = 0;

SimplLoger::SimplLoger():
	logFileName("logFile.txt")
{
	if(!logOut.is_open())
	{
		logOut.open(logFileName, std::ios::app);
	}
}

SimplLoger::~SimplLoger()
{
	if (logOut.is_open())
		logOut.close();
}

void SimplLoger::logMessege(const std::string& messeg, const LogLevel level)
{
	if (levelLog <= static_cast<int>(level))
	{
		time_t now{ time(0) };
		tm dt{};
		localtime_s(&dt, &now);
		logOut << std::to_string(dt.tm_mday) + "." + std::to_string(dt.tm_mon + 1) + " - " + std::to_string(dt.tm_hour) + ":" + std::to_string(dt.tm_min) + ":" + std::to_string(dt.tm_sec) << "\t" << messeg << std::endl;
	}
}

void SimplLoger::logInit(const LogLevel targetLevel)
{
	levelLog = static_cast<int>(targetLevel);
}

SimplLoger LOG;