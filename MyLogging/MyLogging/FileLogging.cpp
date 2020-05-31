#include"FileLogging.h"

FileLogging::FileLogging()
{
    SetLoggingType(1);
}

void FileLogging::Logging()
{
    std::cout << "파일 로깅" << std::endl;
}