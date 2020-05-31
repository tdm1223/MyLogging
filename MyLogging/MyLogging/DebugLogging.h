#pragma once
#include "LoggingInterface.h"

class DebugLogging : public LoggingInterface
{
public:
    void Logging() override;
    int GetLoggingType() override;

private:
    const int loggingType;
};

