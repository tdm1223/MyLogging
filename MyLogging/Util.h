#ifndef UTIL
#define UTIL
#include <Windows.h>
#include <string>

const INT MAX_LOG_TYPE = 3;
const INT MAX_OUTPUT_LENGTH = 1024 * 4;
const INT MAX_QUEUE_CNT = 10000;
const INT DEFAULT_TICK = 1000;              // 로깅 갱신 시간
const INT MAX_LOGFILE_SIZE = 1024 * 200;    // 로그 파일 크기
const INT MAX_LOGTYPE = 4;

enum LogInfoType
{
    kNone = 0,
    kInfoLow = 1,
    kInfoNormal = 2,
    kInfoHigh = 3,
    kInfoCritical = 4,
};

enum LogType
{
    kFile = 0,
    kConsole = 1,
    kDebugView = 2,
};

enum class FileLogType : INT
{
    kFileNone = 0,
    kText = 1,
};

// 로그 메시지 구조체
struct LogMsg
{
    LogInfoType logType;                    // 로그 메시지의 타입
    CHAR outputString[MAX_OUTPUT_LENGTH];   // 로그 메시지
};

struct LogConfig
{
    std::vector<INT> logTypes;  // 로그 종류 (LogInfoType의 값을 할당한다)
    CHAR logFileName[MAX_PATH]; // 로그 파일 명
    FileLogType fileLogType;    // 로그 파일 형식
    HWND hWnd;                  // 로그를 남길 윈도우 핸들
    UINT32 processTick;         // Log 처리 시간 (기본 1초)
    LogConfig()
    {
        ZeroMemory(this, sizeof(LogConfig));
        logTypes.assign(MAX_LOG_TYPE, 0);
        processTick = DEFAULT_TICK;
    }
};

static std::vector<std::string> logTypeString = { "LOG_NONE", "LOG_INFO_LOW", "LOG_INFO_NORMAL", "LOG_INFO_HIGH", "LOG_INFO_CRITICAL" };

#endif // !UTIL
