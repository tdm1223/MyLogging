#pragma once
#include "LoggingInterface.h"

class NetworkLogging : public LoggingInterface
{
public:
    void Logging() override
    {
        std::cout << "네트워크 로깅" << std::endl;
    }
};