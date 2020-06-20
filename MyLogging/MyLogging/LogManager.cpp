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
    LONG queueCnt = 0;
    do
    {
        queueCnt = LogManager::GetInstance()->IncrementBufferIndex();
        // 현재 큐 Size를 초과하였다면 
        if (queueCnt == MAX_QUEUE_CNT)
        {
            LogManager::GetInstance()->ResetBufferIndex();
            queueCnt = 0;
        }
        else if (queueCnt > MAX_QUEUE_CNT)
        {
            Sleep(1);
        }
    } while (queueCnt >= MAX_QUEUE_CNT);

    va_list	argptr;
    va_start(argptr, outputString);
    _vsnprintf_s(logMsg[queueCnt].outputString, MAX_OUTPUT_LENGTH, outputString.c_str(), argptr);
    va_end(argptr);
    logMsg[queueCnt].logMsgInfoType = logInfoType;
    LogManager::GetInstance()->PushMsgQueue(&logMsg[queueCnt]);
}

LogManagerImpl::LogManagerImpl()
{
    ZeroMemory(logInfoLevel_, MAX_LOG_TYPE * sizeof(INT));
    windowHandle_ = NULL;
    msgBufferIndex_ = 0;
    isInit_ = FALSE;
}

LogManagerImpl::~LogManagerImpl()
{
    InsertMsgToQueue(kInfoNormal, "SYSTEM | cLogImpl::~cLogImpl() | cLogImpl 소멸자 호출");
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
        LoggingInterface* tmp = new FileLogging(logConfig);
        loggingList_.push_back(tmp);
    }

    loggingList_.push_back(new ConsoleLogging(logConfig));
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

    //로그, 시간 : 정보형태 : 정보레벨 : 클래스 : 함수 : 에러원인
    CHAR szTime[25];
    time_t curTime;
    struct tm localTime;
    curTime = time(NULL);
    localtime_s(&localTime, &curTime);
    strftime(szTime, 25, "%Y/%m/%d(%H:%M:%S)", &localTime);
    outputString[MAX_OUTPUT_LENGTH - 1] = NULL;
    szTime[24] = NULL;

    logInfoTypeTable[logStringIndex][99] = NULL;
    _snprintf_s(outputString_, MAX_OUTPUT_LENGTH * 2, "%s | %s | %s | %s%c%c", szTime, (logInfo >> 4) ? "에러" : "정보", logInfoTypeTable[logStringIndex], outputString, 0x0d, 0x0a);
    
    for (auto log : loggingList_)
    {
        log->Logging(outputString_, localTime);
    }

    if (logLevelByTypes[kDebugView] <= logLevel)
    {
        OutputDebugWnd(outputString_);
    }
}

void LogManagerImpl::CloseAllLog()
{
    std::lock_guard<std::recursive_mutex> lock(lock_);

    //남아있는 로그를 모두 찍는다.
    OnProcess();

    ZeroMemory(logInfoLevel_, MAX_LOG_TYPE * sizeof(INT));
    windowHandle_ = NULL;
    msgBufferIndex_ = 0;

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
    size_t logCount = logQueue_.Size();
    for (UINT32 i = 0; i < logCount; i++)
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

size_t LogManagerImpl::GetQueueSize()
{
    return logQueue_.Size();
}

void LogManagerImpl::InsertMsgToQueue(LogInfoType logInfoType, const CHAR* outputString)
{
    LONG queueCount = InterlockedIncrement((LONG*)&msgBufferIndex_);

    //현재 큐 Size를 초과하였다면 
    if (MAX_QUEUE_CNT == queueCount)
    {
        InterlockedExchange((LONG*)&msgBufferIndex_, 0);
        queueCount = 0;
    }

    strncpy_s(logMsg[queueCount].outputString, outputString, MAX_OUTPUT_LENGTH);
    logMsg[queueCount].logMsgInfoType = logInfoType;
    logQueue_.Push(&logMsg[queueCount]);
}

UINT32 LogManagerImpl::IncrementBufferIndex()
{
    return InterlockedIncrement((LONG*)&msgBufferIndex_);
}

void LogManagerImpl::ResetBufferIndex()
{
    InterlockedExchange((LONG*)&msgBufferIndex_, 0);
}

void LogManagerImpl::PushMsgQueue(LogMsg* logMsg)
{
    logQueue_.Push(logMsg);
}

void LogManagerImpl::SetLogInfoTypes(LogType logType, LogInfoType logInfoType)
{
    logInfoLevel_[logType] = logInfoType;
}

void LogManagerImpl::OutputDebugWnd(CHAR* outputString)
{
    OutputDebugStringA(outputString);
}