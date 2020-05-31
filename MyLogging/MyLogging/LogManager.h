#pragma once
#include<iostream>
#include "LoggingInterface.h"
#include "FileLogging.h"
#include "NetworkLogging.h"
#include "DebugLogging.h"
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

private:
    std::vector<LoggingInterface*> loggingList_;
};

