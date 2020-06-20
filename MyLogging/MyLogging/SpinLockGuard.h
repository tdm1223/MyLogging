#ifndef SPIN_LOCK_GUARD
#define SPIN_LOCK_GUARD

#define WIN32_LEAN_AND_MEAN
#include<windows.h>

const int SPIN_LOCK_COUNT = 4000;

class SpinLock
{
public:
    SpinLock()
    {
        InitializeCriticalSectionAndSpinCount(&syncObject, SPIN_LOCK_COUNT);
    }

    ~SpinLock() {}

    void Enter()
    {
        ::EnterCriticalSection(&syncObject);
    }

    void Leave()
    {
        ::LeaveCriticalSection(&syncObject);
    }

private:
    CRITICAL_SECTION syncObject;
};

class SpinLockGuard
{
public:
    SpinLockGuard(SpinLock& lock) : lock_(lock)
    {
        lock_.Enter();
    }

    ~SpinLockGuard()
    {
        lock_.Leave();
    }

private:
    SpinLock& lock_;
};

#endif // !SPIN_LOCK_GUARD
