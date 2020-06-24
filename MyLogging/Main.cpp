#include<iostream>
#include"LogManager.h"

int main()
{
    LogConfig logConfig;
    strncpy_s(logConfig.logFileName, "Test", MAX_PATH);
    logConfig.maxLoggingLevel[LogType::kFile] = LoggingLevel::kInfoLow;    // file에는 Low 이하의 로깅만 출력              (Low)
    logConfig.maxLoggingLevel[LogType::kDebugView] = LoggingLevel::kInfoNormal; // debugView에는 Normal 이하의 로깅만 출력 (Low, Normal)
    logConfig.maxLoggingLevel[LogType::kConsole] = LoggingLevel::kInfoCritical; // console에는 Critical 이하의 로깅만 출력 (Low, Normal, High, Critical)

    LogManager::InitLog(logConfig);
    while (1)
    {
        int a;
        std::cin >> a;
        switch (a)
        {
        case kInfoLow:
            LogManager::Log(kInfoLow, "테스트1");
            break;
        case kInfoNormal:
            LogManager::Log(kInfoNormal, "테스트2");
            break;
        case kInfoHigh:
            LogManager::Log(kInfoHigh, "테스트3");
            break;
        case kInfoCritical:
            LogManager::Log(kInfoCritical, "테스트4");
            break;
        case 5:
            LogManager::SetEnabled(LogType::kFile, FALSE);
            break;
        case 6:
            LogManager::SetEnabled(LogType::kFile, TRUE);
            break;
        }
    }

    //while (1)
    //{
    //    LogManager::LOG(kInfoLow, "테스트1");
    //    LogManager::LOG(kInfoNormal, "테스트2");
    //    LogManager::LOG(kInfoHigh, "테스트3");
    //    LogManager::LOG(kInfoCritical, "테스트4");
    //}
    LogManager::CloseLog();
    return 0;
}