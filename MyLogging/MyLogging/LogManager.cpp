#include"LogManager.h"

LogManager* LogManager::instance_ = nullptr;

LogManager::LogManager()
{
}

LogManager* LogManager::GetInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new LogManager();
    }
    return instance_;
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
    mutex_.lock();
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
    mutex_.unlock();
}

void LogManager::DeleteLogType(logType type)
{
    mutex_.lock();
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
    mutex_.unlock();
}

void LogManager::Logging()
{
    mutex_.lock();
    std::vector<LoggingInterface*>& loggingList = GetLoggingList();
    for (auto log : loggingList)
    {
        if (log->GetEnabled() == true)
        {
            log->Logging();
        }
    }
    mutex_.unlock();
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