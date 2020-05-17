#include<iostream>
#include "LogManager.h"
#include "LoggingInterface.h"

using namespace std;

int main()
{
    LogManager* manager = new LogManager();
    manager->IsEnabled(true);

    manager->SetInterface(new FileLogging());
    manager->Logging();

    manager->SetInterface(new DebugLogging());
    manager->Logging();

    manager->SetInterface(new NetworkLogging());
    manager->Logging();

    return 0;
}