#include<iostream>
#include "LogManager.h"

int main()
{
    logging::LogManager* manager = new logging::LogManager();
    manager->IsEnabled(true);

    manager->Initialize(logging::kFile);
    manager->Logging();

    manager->Initialize(logging::kDebug);
    manager->Logging();

    manager->Initialize(logging::kNetwork);
    manager->Logging();

    return 0;
}