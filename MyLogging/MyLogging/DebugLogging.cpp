#include"DebugLogging.h"

void DebugLogging::Logging()
{
    std::cout << "디버그 로깅" << std::endl;
}

int DebugLogging::GetLoggingType()
{
    return loggingType;
}
