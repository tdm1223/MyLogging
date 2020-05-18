#pragma once
#include<iostream>
#include "LoggingInterface.h"
#include "FileLogging.h"
#include "NetworkLogging.h"
#include "DebugLogging.h"

using namespace std;

namespace logging
{
    enum logType
    {
        kFile,
        kDebug,
        kNetwork
    };

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
        void Initialize(logType type)
        {
            isEnabled = true;
            if (loggingInterface)
            {
                delete loggingInterface;
            }

            switch (type)
            {
            case kFile:
                loggingInterface = new FileLogging();
                break;
            case kDebug:
                loggingInterface = new DebugLogging();
                break;
            case kNetwork:
                loggingInterface = new NetworkLogging();
                break;
            }
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

}
