#ifndef LOG_QUEUE
#define LOG_QUEUE

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <queue>
#include "SpinLockGuard.h"
#include "Util.h"

// 로그 메시지를 담는 큐
class LogQueue
{
public:
    LogQueue();
    ~LogQueue();
    void Push(LogMsg* item);
    void Pop();
    LogMsg* Front();
    SIZE_T Size();
    void Clear();

private:
    SpinLock lock_;
    std::queue<LogMsg*> logQueue_;
};

#endif // !LOG_QUEUE
