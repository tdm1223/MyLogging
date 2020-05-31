#ifndef __LOGGING_INTERFACE_H__
#define __LOGGING_INTERFACE_H__

#include<iostream>

class LoggingInterface
{
public:
    virtual void Logging() = 0;
    virtual int GetLoggingType() = 0;
};

#endif // !__LOGGING_INTERFACE_H__
