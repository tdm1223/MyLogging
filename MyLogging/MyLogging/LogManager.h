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

    FileLogType fileLogType; // �α� ���� ����
    HWND hWnd; // �α׸� ���� ������ �ڵ�
    UINT32 processTick; // Log ó�� �ð� (�⺻ 1��)
    UINT32 fileMaxSize; // Log���� ������

    LogConfig()
    {
        ZeroMemory(this, sizeof(LogConfig));
        processTick = DEFAULT_TICK;
        fileMaxSize = 1024 * 50000; // 50MB �⺻���� ���� �ִ� 100MB ���� ����
    }
};

//�۷ι� ����
static CHAR outStr[MAX_OUTPUT_LENGTH];
static LogMsg logMsg[MAX_QUEUE_CNT];

//���� �α׸� ������ Ŭ����
class LogManagerImpl : public Thread
{
public:
    LogManagerImpl();
    virtual ~LogManagerImpl();

    //�������̽� �Լ�
    BOOL Init(LogConfig& logConfig);
    void LogOutput(LogInfoType logInfo, CHAR* outputString);
    void CloseAllLog();

    // ������ â �α׸� ������.
    void CloseWindowLog();

    // ������ ó�� �Լ�
    virtual void OnProcess();

    // ť ���� �Լ�
    size_t GetQueueSize();
    void InsertMsgToQueue(LogInfoType logInfoType, const CHAR* outputString); //ť�� �޼����� �߰��Ѵ�. 
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
    FileLogType fileLogType_; //�α� ������ ����
    CHAR outputString_[MAX_OUTPUT_LENGTH * 2];
    LogConfig logConfig_; // �α� ���� ����
    HWND windowHandle_; // Window�� �α׸� ���� ������ �ڵ�
    HANDLE logFileHandle_; //File handle����
    UINT32 fileCount_; //��ġ�� ������ ���� �ϱ� ���ؼ�
    BOOL isInit_;
    LogQueue logQueue_; // �޼��� ť
    INT msgBufferIndex_; //���� �޼��� ���� ��ġ
    UINT32 fileMaxSize_;
    struct tm logFileLocalTime; // ���� �α� ������ ���� �ð�
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
