#ifndef __DEBUG_LOGGING_H__
#define __DEBUG_LOGGING_H__

#include "LoggingInterface.h"

class DebugLogging : public LoggingInterface
{
public:
    DebugLogging();
    void Logging() override;
};

#endif // !__DEBUG_LOGGING_H__
