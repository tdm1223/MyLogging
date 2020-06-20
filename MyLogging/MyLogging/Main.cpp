#include<iostream>
#include"LogManager.h"

int main()
{
    LogConfig logConfig;
    strncpy_s(logConfig.logFileName, "Test", MAX_PATH);
    logConfig.logInfoLevelByTypes[file] = LOG_INFO_LOW;
    logConfig.logInfoLevelByTypes[console] = LOG_INFO_LOW;

    LogManager::INIT_LOG(logConfig);

    while (1)
    {
        int a;
        std::cin >> a;
        switch (a)
        {
        case LOG_INFO_LOW:
            LogManager::LOG(LOG_INFO_LOW, "테스트1");
            break;
        case LOG_INFO_NORMAL:
            LogManager::LOG(LOG_INFO_NORMAL, "테스트2");
            break;
        case LOG_INFO_HIGH:
            LogManager::LOG(LOG_INFO_HIGH, "테스트3");
            break;
        case LOG_INFO_CRITICAL:
            LogManager::LOG(LOG_INFO_CRITICAL, "테스트4");
            break;
        }
    }
    LogManager::CLOSE_LOG();
    return 0;
}