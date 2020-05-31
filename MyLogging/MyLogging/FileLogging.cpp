#include"FileLogging.h"

void FileLogging::Logging()
{
    std::cout << "파일 로깅" << std::endl;
}

int FileLogging::GetLoggingType()
{
    return loggingType;
}
