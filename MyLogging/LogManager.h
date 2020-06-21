#ifndef LOG_MANAGER
#define LOG_MANAGER

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ctime>
#include <mutex>
#include <string>
#include "Singleton.h"
#include "Thread.h"
#include "LogQueue.h"
#include "Util.h"
#include <iostream>

static CHAR logInfoTypeTable[][100] =
{
    "LOG_NONE",
    "LOG_INFO_LOW",
    "LOG_INFO_NORMAL",
    "LOG_INFO_HIGH",
    "LOG_INFO_CRITICAL",
};

class LoggingInterface
{
public:
    virtual BOOL Init()
    {
        return TRUE;
    }
    virtual void Logging(CHAR* outputString, tm localTime, LogInfoType logInfo = LogInfoType::kNone) = 0;
    virtual void Close() {}

protected:
    CHAR outputString_[MAX_OUTPUT_LENGTH * 2];
    LogConfig logConfig_;
    LogInfoType logInfo_;
};

class FileLogging : public LoggingInterface
{
public:
    FileLogging(LogConfig& config)
    {
        fileMaxSize_ = MAX_LOGFILE_SIZE;
        fileLogType_ = FileLogType::kFileNone;
        ZeroMemory(logFileName_, MAX_PATH);
        logConfig_ = config;
        logFileHandle_ = NULL;
        fileCount_ = 0;
    }

    BOOL InitFile()
    {
        struct tm loctime;
        time_t curtime;
        curtime = time(NULL);
        localtime_s(&loctime, &curtime);

        CreateDirectory(L"../LOG", NULL);
        CHAR strtime[100];
        logFileLocalTime_ = loctime;

        strftime(strtime, 100, "%Y%m%d_01", &loctime);

        _snprintf_s(logFileName_, MAX_PATH * 2, "../Log/%s_%s_%02d.log", logConfig_.logFileName, strtime, ++fileCount_);
        logFileHandle_ = CreateFileA(logFileName_, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (logFileHandle_ == NULL)
        {
            return FALSE;
        }
        return TRUE;
    }

    BOOL Init()
    {
        return InitFile();
    }

    void Logging(CHAR* loggingString, tm localTime, LogInfoType logInfo)
    {
        if (logFileHandle_ == nullptr)
        {
            return;
        }

        DWORD writtenBytes = 0;
        UINT32 curFileSize = GetFileSize(logFileHandle_, NULL);
        // 파일 용량이 지정한 크기를 초과헀거나 날짜가 다르다면 새로 만든다.
        if (curFileSize > fileMaxSize_ || localTime.tm_mday != logFileLocalTime_.tm_mday)
        {
            CloseHandle(logFileHandle_);
            logFileHandle_ = NULL;
            InitFile();
        }

        // 파일 끝으로 파일 포인터를 옮긴다. 
        SetFilePointer(logFileHandle_, 0, nullptr, FILE_END);
        WriteFile(logFileHandle_, loggingString, (UINT32)strlen(loggingString), &writtenBytes, NULL);
    }

    void Close()
    {
        fileLogType_ = FileLogType::kFileNone;
        //파일 로그를 끝낸다.
        if (logFileHandle_)
        {
            CloseHandle(logFileHandle_);
            logFileHandle_ = NULL;
        }
    }

private:
    FileLogType fileLogType_;       // 파일의 형식
    struct tm logFileLocalTime_;    // 파일 생성 시간
    CHAR logFileName_[MAX_PATH];    // 파일 이름
    UINT32 fileCount_;              // 겹치는 파일이 없게 하기 위한 파일 카운트
    HANDLE logFileHandle_;          // 파일 핸들
    UINT32 fileMaxSize_;            // 파일 최대 크기
};

class ConsoleLogging : public LoggingInterface
{
public:
    ConsoleLogging(LogConfig& config)
    {
        logConfig_ = config;
    }
    void Logging(CHAR* loggingString, tm localTime, LogInfoType logInfo)
    {
        printf("[%d] - %s", logInfo, loggingString);
    }
};

class DebugLogging : public LoggingInterface
{
public:
    DebugLogging(LogConfig& config)
    {
        logConfig_ = config;
    }
    void Logging(CHAR* loggingString, tm localTime, LogInfoType logInfo)
    {
        OutputDebugStringA(loggingString);
    }
};

// 실제 로그를 구현한 클래스
class LogManagerImpl : public Thread
{
public:
    LogManagerImpl();
    virtual ~LogManagerImpl();

    //인터페이스 함수
    BOOL Init(LogConfig& logConfig);
    void LogOutput(LogInfoType logInfo, CHAR* outputString);
    void CloseAllLog();

    // 윈도우 창 로그를 끝낸다.
    void CloseWindowLog();

    // 쓰레드 처리 함수
    virtual void OnProcess();

    // 큐 관련 함수
    void PushMsgQueue(LogMsg* logMsg);

private:
    std::recursive_mutex lock_;
    INT logInfoLevel_[MAX_LOG_TYPE];
    LogConfig logConfig_;                           // 로그 저장 변수
    HWND windowHandle_;                             // 로그를 남길 윈도우 핸들
    BOOL isInit_;
    LogQueue logQueue_;                             // 메세지 큐
    std::vector<LoggingInterface*> loggingList_;
    CHAR outputString_[MAX_OUTPUT_LENGTH * 2];
};

class LogManager : public LogManagerImpl, public Singleton<LogManager>
{
public:
    LogManager();
    virtual ~LogManager();
    static BOOL INIT_LOG(LogConfig& logConfig);
    static void CLOSE_LOG();
    static void LOG(LogInfoType logInfoType, const std::string outputString, ...);
};

#endif
