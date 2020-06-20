#include "Thread.h"

Thread::Thread()
{
}

Thread::~Thread()
{
    ReleaseResource();
}

BOOL Thread::CreateThread(UINT32 waitTick)
{
    UINT32 uiThreadId = 0;
    threadHandle_ = (HANDLE)_beginthreadex(NULL, 0, &Thread::CallTickThread, this, CREATE_SUSPENDED, &uiThreadId);
    if (threadHandle_ == nullptr)
    {
        return FALSE;
    }
    waitTick_ = waitTick;

    // 이벤트를 생성한다.
    if (quitEvent == nullptr)
    {
        quitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    }
    isRun_ = FALSE;
    isQuit_ = FALSE;
    return TRUE;
}

void Thread::TickThread()
{
    while (!isQuit_)
    {
        UINT32 dwRet = WaitForSingleObject(quitEvent, waitTick_);
        if (WAIT_OBJECT_0 == dwRet)
        {
            if (threadHandle_)
            {
                CloseHandle(threadHandle_);
                threadHandle_ = nullptr;
            }
            break;
        }
        else if (dwRet == WAIT_TIMEOUT)
        {
            tickCount_++;
            if (isQuit_ == FALSE)
            {
                OnProcess();
            }
        }
    }
}

void Thread::WorkerThread()
{
    while (!isQuit_)
    {
        OnProcess();
    }
}

BOOL Thread::DestroyThread()
{
    isQuit_ = TRUE;
    if (NULL != threadHandle_)
    {
        // work thread
        if (waitTick_ == 0)
        {
            Thread::Stop();
            Thread::Run();
        }
        // tick thread
        else
        {
            if (isRun_ == FALSE)
            {
                Thread::Run();
            }
            SetEvent(quitEvent);
        }
        WaitForSingleObject(threadHandle_, INFINITE);
    }
    ReleaseResource();
    return TRUE;
}

BOOL Thread::Run()
{
    if (isRun_ == FALSE)
    {
        isRun_ = TRUE;
        ResumeThread(threadHandle_);
        return TRUE;
    }
    return FALSE;
}

BOOL Thread::Stop()
{
    if (isRun_ == TRUE)
    {
        isRun_ = FALSE;
        SuspendThread(threadHandle_);
        return TRUE;
    }
    isQuit_ = TRUE;
    return FALSE;
}

UINT32 Thread::GetWaitTick()
{
    return waitTick_;
}

BOOL Thread::IsRun()
{
    return isRun_;
}

BOOL Thread::IsQuit()
{
    return isQuit_;
}

void Thread::ReleaseResource()
{
    isQuit_ = TRUE;
    isRun_ = FALSE;
    //이벤트 리소스를 제거
    if (quitEvent)
    {
        CloseHandle(quitEvent);
        quitEvent = nullptr;
    }

    //쓰레드 리소스 제거
    if (threadHandle_)
    {
        CloseHandle(threadHandle_);
        threadHandle_ = nullptr;
    }
}

UINT32 __stdcall Thread::CallTickThread(LPVOID p)
{
    Thread* tickThread = (Thread*)p;
    if (tickThread->GetWaitTick())
    {
        tickThread->TickThread();
    }
    else
    {
        tickThread->WorkerThread();
    }
    return 1;
}