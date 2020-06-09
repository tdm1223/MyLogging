#include"LogManager.h"

LogManager* LogManager::instance = nullptr;

LogManager::LogManager()
{
}

LogManager* LogManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new LogManager();
    }
    return instance;
}

LogManager::~LogManager()
{
    std::vector<LoggingInterface*>& loggingList = GetLoggingList();
    for (auto log : loggingList)
    {
        delete log;
    }
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
    std::vector<LoggingInterface*>& loggingList = GetLoggingList();
    loggingList.push_back(loggingInterface);
}

void LogManager::DeleteLogType(logType type)
{
    std::vector<LoggingInterface*>& loggingList = GetLoggingList();
    auto iter = loggingList.begin();
    for (auto log : loggingList)
    {
        if (log->GetLoggingType() == static_cast<int>(type))
        {
            loggingList.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}

void LogManager::Logging()
{
    std::vector<LoggingInterface*>& loggingList = GetLoggingList();
    for (auto log : loggingList)
    {
        if (log->GetEnabled() == true)
        {
            log->Logging();
        }
    }
}

std::vector<LoggingInterface*>& LogManager::GetLoggingList()
{
    return loggingList_;
}

void LogManager::Disable(logType type)
{
    std::vector<LoggingInterface*>& loggingList = GetLoggingList();
    for (auto log : loggingList)
    {
        if (log->GetLoggingType() == static_cast<int>(type))
        {
            log->SetEnabled(false);
        }
    }
}

void LogManager::Enable(logType type)
{
    std::vector<LoggingInterface*>& loggingList = GetLoggingList();
    for (auto log : loggingList)
    {
        if (log->GetLoggingType() == static_cast<int>(type))
        {
            log->SetEnabled(true);
        }
    }
}