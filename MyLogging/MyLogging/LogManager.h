#pragma once
#include<iostream>
#include "LoggingInterface.h"
#include "FileLogging.h"
#include "NetworkLogging.h"
#include "DebugLogging.h"
#include<vector>

using namespace std;

namespace logging
{
    enum logType
    {
        kFile = 1,
        kDebug = 2,
        kNetwork = 4
    };

    class LogManager
    {
    public:
        LogManager() : loggingInterface(0), isEnabled(false) {}
        LogManager(logType type);
        ~LogManager();
        void Initialize(logType type);
        void IsEnabled(bool isEnabled);
        void Logging();

    private:
        LoggingInterface* loggingInterface;
        bool isEnabled;
    };

    LogManager GetModule(logType type);
    std::vector<LogManager> GetLoggingList();
}
