#include<iostream>
#include"LogManager.h"

int main()
{
    LogConfig logConfig;
    strncpy_s(logConfig.logFileName, "Test", MAX_PATH);
    logConfig.maxLoggingLevel[LogType::kFile] = LoggingLevel::kInfoLow;         // file에는 Low 이하의 로깅만 출력
    logConfig.maxLoggingLevel[LogType::kDebugView] = LoggingLevel::kInfoNormal; // debugView에는 Normal 이하의 로깅만 출력
    logConfig.maxLoggingLevel[LogType::kConsole] = LoggingLevel::kInfoCritical; // console에는 Critical 이하의 로깅만 출력

    LogManager::INIT_LOG(logConfig);

    //while (1)
    //{
    //    int a;
    //    std::cin >> a;
    //    switch (a)
    //    {
    //    case kInfoLow:
    //        LogManager::LOG(kInfoLow, "테스트1");
    //        break;
    //    case kInfoNormal:
    //        LogManager::LOG(kInfoNormal, "테스트2");
    //        break;
    //    case kInfoHigh:
    //        LogManager::LOG(kInfoHigh, "테스트3");
    //        break;
    //    case kInfoCritical:
    //        LogManager::LOG(kInfoCritical, "테스트4");
    //        break;
    //    default:
    //        break;
    //    }
    //}

    while (1)
    {
        LogManager::LOG(kInfoLow, "테스트1");
        LogManager::LOG(kInfoNormal, "테스트2");
        LogManager::LOG(kInfoHigh, "테스트3");
        LogManager::LOG(kInfoCritical, "테스트4");
    }
    LogManager::CLOSE_LOG();
    return 0;
}