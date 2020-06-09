#include<iostream>
#include"LogManager.h"

int main()
{
    LogManager* instance;
    instance = LogManager::GetInstance();
    instance->AddLogType(logType::kDebug);
    instance->Logging();
    std::cout << "=================================" << endl;
   
    instance->DeleteLogType(logType::kDebug);
    instance->AddLogType(logType::kFile);
    instance->AddLogType(logType::kNetwork);
    instance->Logging();
    return 0;
}