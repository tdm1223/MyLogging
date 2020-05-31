#pragma once
#include "LoggingInterface.h"

class FileLogging : public LoggingInterface
{
public:
    void Logging() override;
    int GetLoggingType() override;

private:
    const int loggingType = 1;
};