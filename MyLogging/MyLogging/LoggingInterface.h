#pragma once
#include<iostream>

class LoggingInterface
{
public:
    virtual void Logging() = 0;
    virtual int GetLoggingType() = 0;
};