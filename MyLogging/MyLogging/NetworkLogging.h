#pragma once
#include "LoggingInterface.h"

class NetworkLogging : public LoggingInterface
{
public:
    void Logging() override;
    int GetLoggingType() override;

private:
    const int loggingType = 4;
};