#ifndef UTIL
#define UTIL

const int MAX_STORAGE_TYPE  = 3;
const int MAX_OUTPUT_LENGTH = 1024 * 4;
const int MAX_QUEUE_CNT     = 10000;
const int DEFAULT_TICK      = 1000;          // 로깅 갱신 시간 : 1초
const int MAX_LOGFILE_SIZE  = 1024 * 200000; // 로그 파일 크기 : 200MB
const int MAX_LOGTYPE       = 4;
const int MAX_IP_LENGTH     = 16;

enum LogInfoType
{
    LOG_NONE = 0x00,
    LOG_INFO_LOW = 0x01,
    LOG_INFO_NORMAL = 0x02,
    LOG_INFO_HIGH = 0x03,
    LOG_INFO_CRITICAL = 0x04,
};

enum LogType
{
    file = 0x00,
    console = 0x01,
    debugView = 0x02,
};

enum class FileLogType : int
{
    none = 0x00,
    text = 0x01,
};

// 로그 메시지 구조체
struct LogMsg
{
    LogInfoType logMsgInfoType;
    char outputString[MAX_OUTPUT_LENGTH];
};

#endif // !UTIL
