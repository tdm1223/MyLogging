#include "LogQueue.h"

LogQueue::LogQueue()
{
    Clear();
}

LogQueue::~LogQueue()
{
}

void LogQueue::Push(LogMsg* item)
{
    SpinLockGuard lock(lock_);
    logQueue_.push(item);
}

void LogQueue::Pop()
{
    SpinLockGuard lock(lock_);
    logQueue_.pop();
}

LogMsg* LogQueue::GetFront()
{
    SpinLockGuard lock(lock_);
    if (logQueue_.empty())
    {
        return nullptr;
    }

    return logQueue_.front();
}

INT32 LogQueue::GetSize()
{
    SpinLockGuard lock(lock_);
    return logQueue_.size();
}

void LogQueue::Clear()
{
    SpinLockGuard lock(lock_);
    while (!logQueue_.empty())
    {
        logQueue_.pop();
    }
}
