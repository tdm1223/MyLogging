#include "LogManager.h"

logging::LogManager::LogManager(logType type)
{
    Initialize(type);
}

logging::LogManager::~LogManager()
{
    if (loggingInterface)
    {
        delete loggingInterface;
    }
}

void logging::LogManager::Initialize(logType type)
{
    isEnabled = true;
    if (loggingInterface)
    {
        delete loggingInterface;
    }

    switch (type)
    {
    case kFile:
        loggingInterface = new FileLogging();
        break;
    case kDebug:
        loggingInterface = new DebugLogging();
        break;
    case kNetwork:
        loggingInterface = new NetworkLogging();
        break;
    }
}

void logging::LogManager::IsEnabled(bool isEnabled)
{
    this->isEnabled = isEnabled;
}

void logging::LogManager::Logging()
{
    if (isEnabled)
    {
        loggingInterface->Logging();
    }
}

logging::LogManager logging::GetModule(logType type)
{
    return LogManager(type);
}
