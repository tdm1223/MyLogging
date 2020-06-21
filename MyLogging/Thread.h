#ifndef THREAD
#define THREAD

#include<process.h>
#include<windows.h>

#include<iostream>
class Thread
{
public:
    Thread();
    ~Thread();
    BOOL CreateThread(UINT32 waitTick);     // waitTick이 0이면 WorkerThread로 작동한다.
    void TickThread();                      // 일정 Tick마다 스케줄링 된다.
    void WorkerThread();                    // 계속 스케줄링 된다.
    BOOL DestroyThread();                   // 쓰레드를 파괴한다.
    BOOL Run();                             // 쓰레드를 시작한다.
    BOOL Stop();                            // 쓰레드를 멈춘다.
    void ReleaseResource();                 //할당된 자원을 풀어준다.
    virtual void OnProcess() = 0;           // 상속받은 클래스에서 처리될 일을 정의한다.
    BOOL IsRun();
    BOOL IsQuit();
    UINT32 GetWaitTick();

protected:
    HANDLE threadHandle_ = nullptr;
    HANDLE quitEvent = nullptr;
    UINT32 waitTick_ = 0;
    UINT32 tickCount_ = 0;
    BOOL isRun_ = FALSE;
    BOOL isQuit_ = FALSE;

private:
    static UINT32 WINAPI CallTickThread(LPVOID p);
};

#endif // !THREAD
