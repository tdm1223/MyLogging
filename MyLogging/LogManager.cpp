#include "LogManager.h"
#include <iostream>

LogManager::LogManager()
{
}

LogManager::~LogManager()
{
}

BOOL LogManager::INIT_LOG(LogConfig& logConfig)
{
    return LogManager::GetInstance()->Init(logConfig);
}

void LogManager::CLOSE_LOG()
{
    LogManager::GetInstance()->CloseAllLog();
}

void LogManager::LOG(LogInfoType logInfoType, const std::string outputString, ...)
{
    LogMsg* log = new LogMsg;
    va_list	argptr;
    va_start(argptr, outputString);
    _vsnprintf_s(log->outputString, MAX_OUTPUT_LENGTH, outputString.c_str(), argptr);
    va_end(argptr);
    log->logMsgInfoType = logInfoType;
    LogManager::GetInstance()->PushMsgQueue(log);
}

LogManagerImpl::LogManagerImpl()
{
    ZeroMemory(logInfoLevel_, MAX_LOG_TYPE * sizeof(INT));
    windowHandle_ = NULL;
    isInit_ = FALSE;
}

LogManagerImpl::~LogManagerImpl()
{
    CloseAllLog();
    DestroyThread();
}

BOOL LogManagerImpl::Init(LogConfig& logConfig)
{
    std::lock_guard<std::recursive_mutex> lock(lock_);
    CloseAllLog();
    CopyMemory(logInfoLevel_, logConfig.logInfoLevelByTypes, MAX_LOG_TYPE * sizeof(INT));
    logConfig_ = logConfig;
    windowHandle_ = logConfig.hWnd;
    
    // 파일로그를 설정했다면
    if (logInfoLevel_[kFile] != kNone)
    {
        loggingList_.push_back(new FileLogging(logConfig));
    }

    loggingList_.push_back(new ConsoleLogging(logConfig));
    loggingList_.push_back(new DebugLogging(logConfig));

    for (auto logging : loggingList_)
    {
        logging->Init();
    }

    isInit_ = TRUE;
    CreateThread(logConfig.processTick);
    Run();
    return TRUE;
}

void LogManagerImpl::LogOutput(LogInfoType logInfo, CHAR* outputString)
{
    if (isInit_ == FALSE)
    {
        return;
    }
    //저장 타입에 해당하는 로그 레벨을 가져온다.
    INT logLevel = (INT)logInfo;
    INT logStringIndex = logLevel;
    INT* logLevelByTypes = logInfoLevel_;

    CHAR loggingTime[25];
    time_t curTime;
    curTime = time(NULL);
    struct tm localTime;
    localtime_s(&localTime, &curTime); // curTime을 localTime에 저장
    strftime(loggingTime, 25, "%Y/%m/%d(%H:%M:%S)", &localTime); // 구한 localTime의 형식 조절
    outputString[MAX_OUTPUT_LENGTH - 1] = NULL;
    loggingTime[24] = NULL;

    logInfoTypeTable[logStringIndex][99] = NULL;
    _snprintf_s(outputString_, MAX_OUTPUT_LENGTH * 2, "%s | %s | %s | %s%c%c", loggingTime, (logInfo >> 4) ? "에러" : "정보", logInfoTypeTable[logStringIndex], outputString, 0x0d, 0x0a);
    
    for (auto log : loggingList_)
    {
        log->Logging(outputString_, localTime, logInfo);
    }
}

void LogManagerImpl::CloseAllLog()
{
    std::lock_guard<std::recursive_mutex> lock(lock_);

    //남아있는 로그를 모두 찍음
    OnProcess();

    ZeroMemory(logInfoLevel_, MAX_LOG_TYPE * sizeof(INT));
    windowHandle_ = NULL;

    for (auto logging : loggingList_)
    {
        logging->Close();
    }

    //쓰레드 종료
    Stop();
}

void LogManagerImpl::CloseWindowLog()
{
    windowHandle_ = NULL;
}

void LogManagerImpl::OnProcess()
{
    std::lock_guard<std::recursive_mutex> lock(lock_);
    while (!logQueue_.IsEmpty())
    {
        LogMsg* logMsg = logQueue_.Front();
        if (NULL == logMsg)
        {
            return;
        }
        //로그를 찍는다.
        LogOutput(logMsg->logMsgInfoType, logMsg->outputString);
        logQueue_.Pop();
    }
}

void LogManagerImpl::PushMsgQueue(LogMsg* logMsg)
{
    logQueue_.Push(logMsg);
}
