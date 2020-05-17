#pragma once
#include<iostream>
#include "LoggingInterface.h"
#include "FileLogging.h"
#include "NetworkLogging.h"
#include "DebugLogging.h"

using namespace std;

class LogManager
{
public:
    LogManager() : loggingInterface(0), isEnabled(false) {}
    ~LogManager()
    {
        if (loggingInterface)
        {
            delete loggingInterface;
        }
    }
    void SetInterface(LoggingInterface* _interface)
    {
        if (loggingInterface)
        {
            delete loggingInterface;
        }
        loggingInterface = _interface;
    }

    void IsEnabled(bool isEnabled)
    {
        this->isEnabled = isEnabled;
    }

    void Logging()
    {
        if (isEnabled)
        {
            loggingInterface->Logging();
        }
    }

private:
    LoggingInterface* loggingInterface;
    bool isEnabled;
};

