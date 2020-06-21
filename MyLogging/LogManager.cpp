#include "LogManager.h"
#include <iostream>

LogManager::LogManager()
{
    ZeroMemory(outputString_, MAX_OUTPUT_LENGTH * 2 * sizeof(CHAR));
    logInfoLevel_.resize(MAX_LOG_TYPE);
    windowHandle_ = NULL;
    isInit_ = FALSE;
}

LogManager::~LogManager()
{
    CloseAllLog();
    DestroyThread();
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
    log->logType = logInfoType;
    LogManager::GetInstance()->PushMsgQueue(log);
}

BOOL LogManager::Init(LogConfig& logConfig)
{
    std::lock_guard<std::recursive_mutex> lock(lock_);
    CloseAllLog();
    logConfig_ = logConfig;
    windowHandle_ = logConfig.hWnd;
    logInfoLevel_ = logConfig.logTypes;

    // 파일   로그를 설정했다면
    if (logInfoLevel_[kFile] != kNone)
    {
        loggingList_.push_back(new FileLogging(logConfig));
    }

    if (logInfoLevel_[kConsole] != kNone)
    {
        loggingList_.push_back(new ConsoleLogging(logConfig));
    }

    if (logInfoLevel_[kDebugView] != kNone)
    {
        loggingList_.push_back(new DebugLogging(logConfig));
    }

    for (auto logging : loggingList_)
    {
        logging->Init();
    }

    isInit_ = TRUE;
    CreateThread(logConfig.processTick);
    Run();
    return TRUE;
}

void LogManager::LogOutput(LogInfoType logInfo, CHAR* outputString)
{
    if (isInit_ == FALSE)
    {
        return;
    }

    //저장 타입에 해당하는 로그 레벨을 가져온다.
    INT logLevel = (INT)logInfo;

    CHAR loggingTime[25];
    time_t curTime;
    curTime = time(NULL);
    struct tm localTime;
    localtime_s(&localTime, &curTime); // curTime을 localTime에 저장
    strftime(loggingTime, 25, "%Y/%m/%d(%H:%M:%S)", &localTime); // 구한 localTime의 형식 조절
    outputString[MAX_OUTPUT_LENGTH - 1] = NULL;
    loggingTime[24] = NULL;

    _snprintf_s(outputString_, MAX_OUTPUT_LENGTH * 2, "%s | %s | %s | %s%c%c", loggingTime, "정보", logTypeString[logLevel].c_str(), outputString, 0x0d, 0x0a);
    
    for (auto log : loggingList_)
    {
        log->Logging(outputString_, localTime, logInfo);
    }
}

void LogManager::CloseAllLog()
{
    std::lock_guard<std::recursive_mutex> lock(lock_);

    //남아있는 로그를 모두 찍음
    OnProcess();

    logInfoLevel_.resize(MAX_LOG_TYPE);

    windowHandle_ = NULL;

    for (auto logging : loggingList_)
    {
        logging->Close();
    }

    //쓰레드 종료
    Stop();
}

void LogManager::CloseWindowLog()
{
    windowHandle_ = NULL;
}

void LogManager::OnProcess()
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
        LogOutput(logMsg->logType, logMsg->outputString);
        logQueue_.Pop();
        delete logMsg;
    }
}

void LogManager::PushMsgQueue(LogMsg* logMsg)
{
    logQueue_.Push(logMsg);
}
