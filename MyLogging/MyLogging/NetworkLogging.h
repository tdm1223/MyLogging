#pragma once
#include "LoggingInterface.h"

class NetworkLogging : public LoggingInterface
{
public:
    void Logging() override
    {
        std::cout << "��Ʈ��ũ �α�" << std::endl;
    }
};