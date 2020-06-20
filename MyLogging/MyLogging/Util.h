#ifndef UTIL
#define UTIL
#include <Windows.h>

const INT MAX_LOG_TYPE  = 3;
const INT MAX_OUTPUT_LENGTH = 1024 * 4;
const INT MAX_QUEUE_CNT     = 10000;
const INT DEFAULT_TICK      = 1000;          // 로깅 갱신 시간
const INT MAX_LOGFILE_SIZE  = 1024 * 20000; // 로그 파일 크기
const INT MAX_LOGTYPE       = 4;

enum LogInfoType
{
    kNone           = 0x00,
    kInfoLow        = 0x01,
    kInfoNormal     = 0x02,
    kInfoHigh       = 0x03,
    kInfoCritical   = 0x04,
};

enum LogType
{
    kFile       = 0x00,
    kConsole    = 0x01,
    kDebugView  = 0x02,
};

enum class FileLogType : INT
{
    kFileNone   = 0x00,
    kText       = 0x01,
};

// 로그 메시지 구조체
struct LogMsg
{
    LogInfoType logMsgInfoType;
    CHAR outputString[MAX_OUTPUT_LENGTH];
};

struct LogConfig
{
    INT logInfoLevelByTypes[MAX_LOG_TYPE];
    CHAR logFileName[MAX_PATH];

    FileLogType fileLogType; // 로그 파일 형식
    HWND hWnd; // 로그를 남길 윈도우 핸들
    UINT32 processTick; // Log 처리 시간 (기본 1초)
    UINT32 fileMaxSize; // Log파일 사이즈

    LogConfig()
    {
        ZeroMemory(this, sizeof(LogConfig));
        processTick = DEFAULT_TICK;
        fileMaxSize = MAX_LOGFILE_SIZE;
    }
};

#endif // !UTIL
