#pragma once
#include "LoggingInterface.h"

class FileLogging : public LoggingInterface
{
public:
    void Logging() override
    {
        std::cout << "���� �α�" << std::endl;
    }
};