#include"LogManager.h"

LogManager::LogManager()
{
}

LogManager::~LogManager()
{
}

void LogManager::AddLogType(logType type)
{
    LoggingInterface* loggingInterface;
    switch (type)
    {
    case logType::kFile:
        loggingInterface = new FileLogging();
        break;
    case logType::kDebug:
        loggingInterface = new DebugLogging();
        break;
    case logType::kNetwork:
        loggingInterface = new NetworkLogging();
        break;
    }

}

void LogManager::Logging()
{
}

std::vector<LoggingInterface*> LogManager::GetLoggingList()
{
    return loggingList_;
}