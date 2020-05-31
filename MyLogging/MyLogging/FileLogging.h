#ifndef __FILE_LOGGING_H__
#define __FILE_LOGGING_H__

#include "LoggingInterface.h"

class FileLogging : public LoggingInterface
{
public:
    void Logging() override;
    int GetLoggingType() override;

private:
    const int loggingType = 1;
};

#endif // !__FILE_LOGGING_H__
