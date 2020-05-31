#ifndef __FILE_LOGGING_H__
#define __FILE_LOGGING_H__

#include "LoggingInterface.h"

class FileLogging : public LoggingInterface
{
public:
    FileLogging();
    void Logging() override;
};

#endif // !__FILE_LOGGING_H__
