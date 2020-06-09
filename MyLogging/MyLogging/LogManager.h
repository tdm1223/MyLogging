#ifndef __LOG_MANAGER_H__
#define __LOG_MANAGER_H__

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
    static LogManager* GetInstance();
    ~LogManager();

    void AddLogType(logType type);
    void DeleteLogType(logType type);

    void Logging();
    std::vector<LoggingInterface*>& GetLoggingList();

    void Disable(logType type);
    void Enable(logType type);

private:
    LogManager();
    static LogManager* instance;
    std::vector<LoggingInterface*> loggingList_;
};


#endif // !__LOG_MANAGER_H__
