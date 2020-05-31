#ifndef __LOG_MANAGER_H__
#define __LOG_MANAGER_H__

#include<iostream>
#include"LoggingInterface.h"
#include"FileLogging.h"
#include"NetworkLogging.h"
#include"DebugLogging.h"
#include<vector>

using namespace std;


enum class logType
{
    kFile = 1,
    kDebug = 2,
    kNetwork = 4
};

class LogManager
{
public:
    LogManager();
    ~LogManager();
    void AddLogType(logType type);
    void Logging();
    std::vector<LoggingInterface*> GetLoggingList();

private:
    std::vector<LoggingInterface*> loggingList_;
};

#endif // !__LOG_MANAGER_H__
