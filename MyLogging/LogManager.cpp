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

void LogManager::LOG(LoggingLevel logInfoType, const std::string outputString, ...)
{
    LogMsg* log = new LogMsg;
    log->logType = logInfoType; // 로그 종류 설정

    // 로그 스트링 설정
    va_list	argptr;
    va_start(argptr, outputString);
    _vsnprintf_s(log->outputString, MAX_OUTPUT_LENGTH, outputString.c_str(), argptr);
    va_end(argptr);

    LogManager::GetInstance()->PushMsgQueue(log);
}

BOOL LogManager::Init(LogConfig& logConfig)
{
    std::lock_guard<std::recursive_mutex> lock(lock_);
    CloseAllLog();
    logConfig_ = logConfig;
    windowHandle_ = logConfig.hWnd;
    logInfoLevel_ = logConfig.maxLoggingLevel;

    // 파일 로그 설정
    loggingList_.push_back(new FileLogging(logConfig));

    // 콘솔 로그 설정
    loggingList_.push_back(new ConsoleLogging(logConfig));

    // 디버그 로그 설정
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

void LogManager::LogOutput(LoggingLevel logInfo, CHAR* outputString)
{
    if (isInit_ == FALSE)
    {
        return;
    }

    //저장 타입에 해당하는 로그 레벨을 가져온다.
    INT loggingLevel = (INT)logInfo;

    // 로그 시간 설정
    CHAR loggingTime[25];
    time_t curTime;
    curTime = time(NULL);
    struct tm localTime;
    localtime_s(&localTime, &curTime); // curTime을 localTime에 저장
    strftime(loggingTime, 25, "%Y/%m/%d(%H:%M:%S)", &localTime); // 구한 localTime의 형식 조절
    loggingTime[24] = NULL;

    // 로그 스트링 설정
    outputString[MAX_OUTPUT_LENGTH - 1] = NULL;
    _snprintf_s(outputString_, MAX_OUTPUT_LENGTH * 2, "%s | %s | %s\n", loggingTime, loggingLevelString[loggingLevel].c_str(), outputString);

    for (auto log : loggingList_)
    {
        log->Logging(outputString_, localTime, logInfo);
    }
}

void LogManager::CloseAllLog()
{
    std::lock_guard<std::recursive_mutex> lock(lock_);
    OnProcess();
    logInfoLevel_.resize(MAX_LOG_TYPE, 0);
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
        if (logMsg == NULL)
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
