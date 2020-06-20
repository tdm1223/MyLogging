#ifndef UTIL
#define UTIL

const INT MAX_STORAGE_TYPE  = 3;
const INT MAX_OUTPUT_LENGTH = 1024 * 4;
const INT MAX_QUEUE_CNT     = 10000;
const INT DEFAULT_TICK      = 1000;          // 로깅 갱신 시간 : 1초
const INT MAX_LOGFILE_SIZE  = 1024 * 200000; // 로그 파일 크기 : 200MB
const INT MAX_LOGTYPE       = 4;
const INT MAX_IP_LENGTH     = 16;

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

enum class FileLogType : INT
{
    none = 0x00,
    text = 0x01,
};

// 로그 메시지 구조체
struct LogMsg
{
    LogInfoType logMsgInfoType;
    CHAR outputString[MAX_OUTPUT_LENGTH];
};

#endif // !UTIL
