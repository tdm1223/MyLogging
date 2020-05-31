#include "NetworkLogging.h"

void NetworkLogging::Logging()
{
    std::cout << "네트워크 로깅" << std::endl;
}

int NetworkLogging::GetLoggingType()
{
    return loggingType;
}
