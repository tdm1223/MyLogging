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
        // ���� ť Size�� �ʰ��Ͽ��ٸ� 
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
    ZeroMemory(logInfoLevel_, MAX_STORAGE_TYPE * sizeof(INT));
    ZeroMemory(logFileName_, MAX_PATH);
    fileLogType_ = FileLogType::none;
    windowHandle_ = NULL;
    logFileHandle_ = NULL;
    msgBufferIndex_ = 0;
    fileMaxSize_ = 0;
    fileCount_ = 0;
    isInit_ = FALSE;
}

LogManagerImpl::~LogManagerImpl()
{
    InsertMsgToQueue(LOG_INFO_NORMAL, "SYSTEM | cLogImpl::~cLogImpl() | cLogImpl �Ҹ��� ȣ��");
    CloseAllLog();
    DestroyThread();
}

BOOL LogManagerImpl::Init(LogConfig& logConfig)
{
    std::lock_guard<std::recursive_mutex> lock(lock_);
    CloseAllLog();
    CHAR strtime[100];
    CopyMemory(logInfoLevel_, logConfig.logInfoLevelByTypes, MAX_STORAGE_TYPE * sizeof(INT));
    time_t curtime;
    struct tm localTime;
    curtime = time(NULL);
    localtime_s(&localTime, &curtime);
    strftime(strtime, 100, "%Y%m%d", &localTime);
    CreateDirectory(L"./LOG", NULL);
    logConfig_ = logConfig;
    _snprintf_s(logFileName_, MAX_PATH, "%s_%s", logConfig.logFileName, strtime);
    fileMaxSize_ = logConfig.fileMaxSize;
    windowHandle_ = logConfig.hWnd;
    BOOL isError = FALSE;

    // ���Ϸα׸� �����ߴٸ�
    if (logInfoLevel_[file] != LOG_NONE)
    {
        isError = InitFile(localTime);
    }

    //// ���� �߻�
    if (isError == FALSE)
    {
        CloseAllLog();
        return FALSE;
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
    //���� Ÿ�Կ� �ش��ϴ� �α� ������ �����´�.
    INT logLevel = (INT)logInfo;
    INT logStringIndex = logLevel;
    INT* logLevelByTypes = logInfoLevel_;

    //�α�, �ð� : �������� : �������� : Ŭ���� : �Լ� : ��������
    //����ð��� ���´�.
    CHAR szTime[25];
    time_t curTime;
    struct tm localTime;
    //LOG ENUM�� StringTable���� ������ ��ġ ��Ų��.
    curTime = time(NULL);
    localtime_s(&localTime, &curTime);
    strftime(szTime, 25, "%Y/%m/%d(%H:%M:%S)", &localTime);
    outputString[MAX_OUTPUT_LENGTH - 1] = NULL;
    szTime[24] = NULL;

    szLogInfoType_StringTable[logStringIndex][99] = NULL;
    _snprintf_s(outputString_, MAX_OUTPUT_LENGTH * 2, "%s | %s | %s | %s%c%c", szTime, (logInfo >> 4) ? "����" : "����", szLogInfoType_StringTable[logStringIndex], outputString, 0x0d, 0x0a);

    if (logLevelByTypes[file] <= logLevel)
    {
        OutputFile(outputString_, localTime);
    }
    if (logLevelByTypes[console] <= logLevel)
    {
        OutputConsole(logInfo, outputString_);
    }
    if (logLevelByTypes[debugView] <= logLevel)
    {
        OutputDebugWnd(outputString_);
    }
}

void LogManagerImpl::CloseAllLog()
{
    std::lock_guard<std::recursive_mutex> lock(lock_);

    //�����ִ� �α׸� ��� ��´�.
    OnProcess();

    ZeroMemory(logInfoLevel_, MAX_STORAGE_TYPE * sizeof(INT));
    ZeroMemory(logFileName_, MAX_PATH);
    fileLogType_ = FileLogType::none;
    windowHandle_ = NULL;
    msgBufferIndex_ = 0;

    //���� �α׸� ������.
    if (logFileHandle_)
    {
        CloseHandle(logFileHandle_);
        logFileHandle_ = NULL;
    }

    //������ ����
    Stop();
}

void LogManagerImpl::CloseWindowLog()
{
    windowHandle_ = NULL;
}

void LogManagerImpl::OnProcess()
{
    std::lock_guard<std::recursive_mutex> lock(lock_);
    size_t logCount = logQueue_.GetSize();
    for (UINT32 i = 0; i < logCount; i++)
    {
        LogMsg* pLogMsg = logQueue_.GetFront();
        if (NULL == pLogMsg)
        {
            return;
        }
        //�α׸� ��´�.
        LogOutput(pLogMsg->logMsgInfoType, pLogMsg->outputString);
        logQueue_.Pop();
    }
}

size_t LogManagerImpl::GetQueueSize()
{
    return logQueue_.GetSize();
}

void LogManagerImpl::InsertMsgToQueue(LogInfoType logInfoType, const CHAR* outputString)
{
    LONG queueCount = InterlockedIncrement((LONG*)&msgBufferIndex_);

    //���� ť Size�� �ʰ��Ͽ��ٸ� 
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

CHAR* LogManagerImpl::GetLogFileName()
{
    return logFileName_;
}

void LogManagerImpl::SetLogInfoTypes(LogType logType, LogInfoType logInfoType)
{
    logInfoLevel_[logType] = logInfoType;
}

void LogManagerImpl::OutputFile(CHAR* outputString, tm localTime)
{
    if (logFileHandle_ == nullptr)
    {
        return;
    }

    DWORD dwWrittenBytes = 0;
    UINT32 dwSize = 0;
    dwSize = GetFileSize(logFileHandle_, NULL);

    // ���� �뷮�� ���ѿ� �ɷȴٸ�, ��¥�� �ٸ��ٸ� ���� �����.
    if (dwSize > fileMaxSize_ || dwSize > MAX_LOGFILE_SIZE || localTime.tm_mday != logFileLocalTime.tm_mday)
    {
        CHAR strtime[100];
        time_t curtime;
        struct tm loctime;
        curtime = time(NULL);
        localtime_s(&loctime, &curtime);
        strftime(strtime, 100, "%Y%m%d_01", &loctime);
        _snprintf_s(logFileName_, MAX_PATH * 2, "%s_%s_%02d", logConfig_.logFileName, strtime, ++fileCount_);
        CloseHandle(logFileHandle_);
        logFileHandle_ = NULL;
        InitFile(loctime);
    }

    // ���� ������ ���� �����͸� �ű��. 
    SetFilePointer(logFileHandle_, 0, nullptr, FILE_END);
    WriteFile(logFileHandle_, outputString, (UINT32)strlen(outputString), &dwWrittenBytes, NULL);
}

void LogManagerImpl::OutputConsole(LogInfoType logInfo, CHAR* outputString)
{
    printf("[%d] - %s", logInfo, outputString);
}

void LogManagerImpl::OutputDebugWnd(CHAR* outputString)
{
    OutputDebugStringA(outputString);
}

BOOL LogManagerImpl::InitFile(tm curLocalTime)
{
    logFileLocalTime = curLocalTime;
    _snprintf_s(outputString_, MAX_OUTPUT_LENGTH, "./Log/%s.log", logFileName_);
    logFileHandle_ = CreateFileA(outputString_, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (logFileHandle_ == NULL)
    {
        return FALSE;
    }

    fileCount_ = 0;
    return TRUE;
}
