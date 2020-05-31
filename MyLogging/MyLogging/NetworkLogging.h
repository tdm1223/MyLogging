#ifndef __NETWORK_LOGGING_H__
#define __NETWORK_LOGGING_H__

#include "LoggingInterface.h"

class NetworkLogging : public LoggingInterface
{
public:
    void Logging() override;
    int GetLoggingType() override;

private:
    const int loggingType = 4;
};

#endif // !__NETWORK_LOGGING_H__
