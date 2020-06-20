#ifndef __LOGGING_INTERFACE_H__
#define __LOGGING_INTERFACE_H__

#include<iostream>
#include "Util.h"

class LoggingInterface
{
public:
    virtual void Logging() = 0;
    bool GetEnabled();
    void SetEnabled(bool enabled);
    int GetLoggingType();
    void SetLoggingType(int loggingType);

private:
    bool isEnabled_ = true;
    int loggingType_ = 0;
};

#endif // !__LOGGING_INTERFACE_H__
