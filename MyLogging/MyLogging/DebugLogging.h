#ifndef __DEBUG_LOGGING_H__
#define __DEBUG_LOGGING_H__

#include "LoggingInterface.h"

class DebugLogging : public LoggingInterface
{
public:
    void Logging() override;
    int GetLoggingType() override;

private:
    const int loggingType = 2;
};

#endif // !__DEBUG_LOGGING_H__
