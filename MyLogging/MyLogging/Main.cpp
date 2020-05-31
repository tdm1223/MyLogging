#include<iostream>
#include"LogManager.h"

int main()
{
    LogManager* manager = new LogManager();
    manager->AddLogType(logType::kDebug);
    manager->Logging();
    std::cout << "=================================" << endl;
   
    manager->DeleteLogType(logType::kDebug);
    manager->Logging();
    return 0;
}