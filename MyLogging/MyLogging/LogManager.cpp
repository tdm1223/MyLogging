#include "LogManager.h"


LogManager::LogManager()
{
}

LogManager::~LogManager()
{
}

void LogManager::AddLogType(logType type)
{
    LoggingInterface* tmp;
    switch (type)
    {
    case logType::kFile:
        tmp = new FileLogging();
        break;
    case logType::kDebug:
        tmp = new DebugLogging();
        break;
    case logType::kNetwork:
        tmp = new NetworkLogging();
        break;
    }
}

void LogManager::Logging()
{
}
