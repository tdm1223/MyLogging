#pragma once
#include "LoggingInterface.h"

class DebugLogging : public LoggingInterface
{
public:
    void Logging() override
    {
        std::cout << "µð¹ö±× ·Î±ë" << std::endl;
    }
};

