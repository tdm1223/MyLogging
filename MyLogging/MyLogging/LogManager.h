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

static CHAR szLogInfoType_StringTable[][100] =
{
    "LOG_NONE",
    "LOG_INFO_LOW",
    "LOG_INFO_NORMAL",
    "LOG_INFO_HIGH",
    "LOG_INFO_CRITICAL",
};

struct LogConfig
{
    INT logInfoLevelByTypes[MAX_STORAGE_TYPE];
    CHAR logFileName[MAX_PATH];

    FileLogType fileLogType; // 로그 파일 형식
    HWND hWnd; // 로그를 남길 윈도우 핸들
    UINT32 processTick; // Log 처리 시간 (기본 1초)
    UINT32 fileMaxSize; // Log파일 사이즈

    LogConfig()
    {
        ZeroMemory(this, sizeof(LogConfig));
        processTick = DEFAULT_TICK;
        fileMaxSize = 1024 * 50000; // 50MB 기본으로 설정 최대 100MB 까지 가능
    }
};

//글로벌 변수
static CHAR outStr[MAX_OUTPUT_LENGTH];
static LogMsg logMsg[MAX_QUEUE_CNT];

//실제 로그를 구현한 클래스
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
    size_t GetQueueSize();
    void InsertMsgToQueue(LogInfoType logInfoType, const CHAR* outputString); //큐에 메세지를 추가한다. 
    UINT32 IncrementBufferIndex();
    void ResetBufferIndex();
    void PushMsgQueue(LogMsg* logMsg);
    CHAR* GetLogFileName();
    void SetLogInfoTypes(LogType logType, LogInfoType logInfoType);

private:
    void OutputFile(CHAR* outputString, struct tm localTime);
    void OutputConsole(LogInfoType logInfo, CHAR* outputString);
    void OutputDebugWnd(CHAR* outputString);
    BOOL InitFile(struct tm	curLocalTime);

private:
    std::recursive_mutex lock_;
    INT logInfoLevel_[MAX_STORAGE_TYPE];
    CHAR logFileName_[MAX_PATH];
    FileLogType fileLogType_; //로그 파일의 형식
    CHAR outputString_[MAX_OUTPUT_LENGTH * 2];
    LogConfig logConfig_; // 로그 저장 변수
    HWND windowHandle_; // Window로 로그를 남길 윈도우 핸들
    HANDLE logFileHandle_; //File handle변수
    UINT32 fileCount_; //겹치는 파일이 없게 하기 위해서
    BOOL isInit_;
    LogQueue logQueue_; // 메세지 큐
    INT msgBufferIndex_; //현재 메세지 버퍼 위치
    UINT32 fileMaxSize_;
    struct tm logFileLocalTime; // 현재 로그 파일의 생성 시간
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
