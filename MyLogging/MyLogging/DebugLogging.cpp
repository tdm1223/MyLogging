﻿#include"DebugLogging.h"

DebugLogging::DebugLogging()
{
    SetLoggingType(2);
}

void DebugLogging::Logging()
{
    std::cout << "디버그 로깅" << std::endl;
}